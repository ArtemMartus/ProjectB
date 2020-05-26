//
// Created by Artem Martus on 06.04.2020.
//

#include <Checkboard.h>
#include <Figure.h>
#include <FigureFactory.h>
#include <PathSystem.h>
#include <Point.h>
#include <cstdlib>
#include <list>

#ifdef _WIN32
#include <stdexcept>
#endif

using namespace std;

Checkboard::Checkboard()
{
    m_board.clear();
    m_pathSystem = make_shared<PathSystem>(m_board);
}

Checkboard::~Checkboard() { destroy(); }

shared_ptr<Figure> Checkboard::at(const shared_ptr<Point>& point) const
{
    if (!point || !point->inBounds())
        return nullptr;

    for (const auto& item : m_board)
        if (*(item->getPoint()) == *point)
            return item;
    return nullptr;
}

bool Checkboard::prepareMove(const shared_ptr<Point>& from,
    const shared_ptr<Point>& to)
{
    // recheck checkbox for ally figure
    auto figure = at(from);
    if (!figure)
        return false;

    if (m_pathSystem->getBoard().size() != m_board.size())
        m_pathSystem->setBoard(m_board);

    if (!m_pathSystem->checkForMovement(figure, to))
        return false;

    performMovement(figure, to);

    // make aliases for readability
    const auto side = figure->getPlayer();
    const auto type = figure->getType();
    const int endY = figure->getPlayer() == Whites ? 7 : 0;

    // after the movement we update board figures if needed and check special
    // morphs for pawns
    if (type == Pawn && to->getY() == endY) {
        shared_ptr<Figure> undead;
        int temp = -1; // get the most valuable from dead list
        for (const auto& i : m_deadFigures) {
            if (i->getPlayer() != side)
                continue;
            if (i->getType() > temp) {
                undead = i;
                temp = i->getType();
            }
        }

        if (!undead)
            undead = FigureFactory::buildQueen(figure->getPlayer());

        undead->moved();
        undead->revive();
        m_deadFigures.remove(undead);

        undead->getPoint()->setX(to->getX());
        undead->getPoint()->setY(to->getY());
        m_board.push_back(undead);
        figure->isCapturedBy(undead);
        m_board.remove(figure);
        m_deadFigures.push_back(figure);
    }
    m_pathSystem->setBoard(m_board);
    /// need to update pathfinding board after
    /// making morphs and castlings

    return true;
}

bool Checkboard::onePlayerLeft() const
{
    auto side = m_board.front()->getPlayer();
    for (const auto& item : m_board)
        if (item->getPlayer() != side)
            return false;
    return true;
}

void Checkboard::initialize()
{
    if (!m_board.empty() || !m_deadFigures.empty())
        destroy();
    auto blacks = FigureFactory::buildSide(FigurePlayer::Blacks);
    auto whites = FigureFactory::buildSide(FigurePlayer::Whites);

    m_board.splice(m_board.end(), whites);
    m_board.splice(m_board.end(), blacks);
}

void Checkboard::destroy()
{
    m_board.clear();
    m_deadFigures.clear();
}

// save-load block

Checkboard::Checkboard(const list<shared_ptr<Figure>>& figures) noexcept
{
    for (const auto& item : figures) {
        if (item->isAlive())
            m_board.push_back(item);
        else
            m_deadFigures.push_back(item);
    }
    m_pathSystem = make_shared<PathSystem>(m_board);
}

list<shared_ptr<Figure>> Checkboard::getAllFigures() const
{
    list<shared_ptr<Figure>> out;
    out.insert(out.end(), m_board.begin(), m_board.end());
    out.insert(out.end(), m_deadFigures.begin(), m_deadFigures.end());

    return out;
}

void Checkboard::setTurn(bool w) { whitesTurn = w; }

bool Checkboard::getWhitesTurn() const { return whitesTurn; }

void Checkboard::performMovement(const shared_ptr<Figure>& figure,
    const shared_ptr<Point>& to)
{
    if (!figure)
        throw invalid_argument("Cannot perform movement on nullptr");

    auto possibleFigure = at(to);
    if (figure->isKing() && !possibleFigure
        /// if we move with a king and
        /// possibleFigure wasn't found at
        /// that point,
        /// but somehow we got into here than it means 99% that it's castling
        && figure->isReadyForCastling() && abs((int)to->getX() - figure->getX()) > 1) {
        if (to->getX() < figure->getX())
            to->setX(0);
        else
            to->setX(7);

        possibleFigure = at(to); /// but we recheck if nothing is broken, who knows
        if (!possibleFigure || !possibleFigure->isReadyForCastling() || !possibleFigure->isRook())
            throw runtime_error("Something went wrong");
    }

    if (possibleFigure) {
        if (possibleFigure->getPlayer() != figure->getPlayer()) {
            possibleFigure->isCapturedBy(figure);
            possibleFigure->moved();
            m_board.remove(possibleFigure);
            m_deadFigures.push_back(possibleFigure);

        } else { /// else we castle
            shared_ptr<Figure> king, rook;
            if (possibleFigure->isKing()) {
                king = possibleFigure;
                rook = figure;
            } else {
                king = figure;
                rook = possibleFigure;
            }

            int leftCastling = king->getPoint()->getX() > rook->getPoint()->getX() ? -1 : 1;

            king->getPoint()->setX(king->getPoint()->getX() + 2 * leftCastling);
            rook->getPoint()->setX(king->getPoint()->getX() - 1 * leftCastling);
            rook->moved();
            king->moved();
            return;
        }
    }

    figure->getPoint()->setX(to->getX());
    figure->getPoint()->setY(to->getY());
    figure->moved();
}

multimap<shared_ptr<Figure>, shared_ptr<Point>> Checkboard::canMoveFrom(
    FigurePlayer side) const
{
    if (m_pathSystem->getBoard().size() != m_board.size())
        m_pathSystem->setBoard(m_board);
    return m_pathSystem->getListOfAvailableMoves(side);
}
