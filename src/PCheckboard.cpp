//
// Created by Artem Martus on 06.04.2020.
//


#include "PCheckboard.h"
#include "PFigure.h"
#include "PPoint.h"
#include <list>
#include "PPathSystem.h"
#include "PFigureFactory.h"
#include <stdexcept>

using namespace std;

PCheckboard::PCheckboard() {
	m_board.clear();
	m_pathSystem = make_shared<PPathSystem>(m_board);
}

PCheckboard::~PCheckboard() {
	destroy();
}

shared_ptr<PFigure> PCheckboard::at(const shared_ptr<PPoint> &point) const {
	if (!point || !point->inBounds()) return nullptr;

	for (const auto &item: m_board)
		if (*(item->getPoint()) == *point)
			return item;
	return nullptr;
}

bool PCheckboard::prepareMove(const shared_ptr<PPoint> &from, const shared_ptr<PPoint> &to) {
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

	// after the movement we update board figures if needed and check special morphs for pawns
	if (type == Pawn && !m_deadFigures.empty()) {
		int endY = side == Whites ? 7 : 0;
		if (to->getY() == endY) {
			shared_ptr<PFigure> undead;
			int temp = -1; // get the most valuable from dead list
			for (const auto &i: m_deadFigures) {
				if (i->getPlayer() != side) continue;
				if (i->getType() > temp) {
					undead = i;
					temp = i->getType();
				}
			}

			if (!undead) {
				undead = PFigureFactory::buildQueen(figure->getPlayer());
				undead->getPoint()->setX(to->getX());
				undead->getPoint()->setY(to->getY());
			}

			undead->revive();
			undead->moved();
			undead->getPoint()->setX(to->getX());
			undead->getPoint()->setY(to->getY());
			m_deadFigures.remove(undead);
			m_board.push_back(undead);
			figure->capture(undead);
			m_deadFigures.push_back(figure);
			m_board.remove(figure);

		}
	}

	return true;
}

bool PCheckboard::onePlayerLeft() const {
	auto side = m_board.front()->getPlayer();
	for (const auto &item: m_board)
		if (item->getPlayer() != side) return false;
	return true;
}

void PCheckboard::initialize() {
	if (!m_board.empty() || !m_deadFigures.empty())
		destroy();
	auto blacks = PFigureFactory::buildSide(FigurePlayer::Blacks);
	auto whites = PFigureFactory::buildSide(FigurePlayer::Whites);

	m_board.splice(m_board.end(), whites);
	m_board.splice(m_board.end(), blacks);
}


void PCheckboard::destroy() {
	m_board.clear();
	m_deadFigures.clear();
}


// save-load block

PCheckboard::PCheckboard(const list<shared_ptr<PFigure>> &figures) noexcept {
	for (const auto &item: figures) {
		if (item->isAlive())
			m_board.push_back(item);
		else
			m_deadFigures.push_back(item);
	}
	m_pathSystem = make_shared<PPathSystem>(m_board);
}

list<shared_ptr<PFigure>> PCheckboard::getAllFigures() const {
	list<shared_ptr<PFigure>> out;
	out.insert(out.end(), m_board.begin(), m_board.end());
	out.insert(out.end(), m_deadFigures.begin(), m_deadFigures.end());

	return out;
}


void PCheckboard::setTurn(bool w) {
	whitesTurn = w;
}

bool PCheckboard::getWhitesTurn() const {
	return whitesTurn;
}

void PCheckboard::performMovement(const shared_ptr<PFigure> &figure, const shared_ptr<PPoint> &to) {
	if (!figure) throw invalid_argument("Cannot perform movement on nullptr");

	auto possibleFigure = at(to);

	if (possibleFigure) {
		if (possibleFigure->getPlayer() != figure->getPlayer()) {
			possibleFigure->capture(figure);
			possibleFigure->moved();
			m_board.remove(possibleFigure);
			m_deadFigures.push_back(possibleFigure);

		} else { /// else we castle
			shared_ptr<PFigure> king, rook;
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


multimap<shared_ptr<PFigure>, shared_ptr<PPoint>> PCheckboard::canMoveFrom(FigurePlayer side) const {
	if (m_pathSystem->getBoard().size() != m_board.size())
		m_pathSystem->setBoard(m_board);
	return m_pathSystem->getListOfAvailableMoves(side);
}
