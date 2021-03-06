//
// Created by Artem Martus on 10.04.2020.
//

#define BUILD_PATH_INTRO                                                \
    list<shared_ptr<Point>> path;                                       \
    int x = figure->getPoint()->getX(), y = figure->getPoint()->getY(); \
    auto side = figure->getPlayer();

#include <Figure.h>
#include <PathSystem.h>
#include <Point.h>
#include <list>
#include <map>
#include <set>

#ifdef _WIN32

#include <algorithm> // min max functions
#include <stdexcept> // std exceptions

#endif

using namespace std;

PathSystem::PathSystem(std::list<std::shared_ptr<Figure>> b) noexcept
    : board(std::move(b))
{
}

#ifdef _WIN32

PathSystem::PathSystem() noexcept {};

#else

PathSystem::PathSystem() noexcept = default;

#endif

list<shared_ptr<Point>> PathSystem::buildPath(
    const shared_ptr<Figure>& figure) const
{
    if (!figure)
        throw invalid_argument("Cannot build path for nullptr");

    list<shared_ptr<Point>> rawMoves;

    // build possible path for different figure types
    if (figure->isPawn())
        rawMoves.splice(rawMoves.end(), buildPawnPath(figure));

    if (figure->isRook() || figure->isQueen())
        rawMoves.splice(rawMoves.end(), buildRookPath(figure));

    if (figure->isKnight())
        rawMoves.splice(rawMoves.end(), buildKnightPath(figure));

    if (figure->isBishop() || figure->isQueen())
        rawMoves.splice(rawMoves.end(), buildBishopPath(figure));

    if (figure->isKing()) {
        rawMoves.splice(rawMoves.end(), buildKingPath(figure));
    }

    /// there may be so many of them, not really want to break any iterators
    list<shared_ptr<Point>> possibleMoves;
    for (const auto& i : rawMoves)
        if (i)
            possibleMoves.push_back(i);

    return possibleMoves;
}

shared_ptr<Point> PathSystem::addOrDie(const shared_ptr<Point>& p,
    FigurePlayer side,
    bool pawnMode) const
{
    if (!p)
        return nullptr;

    auto fig = at(p);
    bool differentSides = fig && fig->getPlayer() != side;
    // pawns can capture on diagonals but not vertically
    bool allowNext = p->inBounds() && (pawnMode ? (fig && differentSides) : (!fig || differentSides));

    return allowNext ? p : nullptr;
}

list<shared_ptr<Point>> PathSystem::buildPawnPath(
    const shared_ptr<Figure>& figure) const
{
    BUILD_PATH_INTRO;
    auto pawnY = side == FigurePlayer::Whites ? 1 : -1;

    auto p = addOrDie(make_shared<Point>(x, y + pawnY), side,
        false); // cannot attack forward
    if (p && !at(p)) {
        path.push_back(p);
        if (figure->getMovesCount() == 0)
            path.push_back(
                addOrDie(make_shared<Point>(x, y + 2 * pawnY), side, false));
    }

    path.push_back(addOrDie(make_shared<Point>(x + 1, y + pawnY), side, true));
    path.push_back(addOrDie(make_shared<Point>(x - 1, y + pawnY), side, true));

    return path;
}

list<shared_ptr<Point>> PathSystem::buildKnightPath(
    const shared_ptr<Figure>& figure) const
{
    BUILD_PATH_INTRO;

    /// to the right
    path.push_back(addOrDie(make_shared<Point>(x + 2, y + 1), side, false));
    path.push_back(addOrDie(make_shared<Point>(x + 2, y - 1), side, false));

    /// to the left
    path.push_back(addOrDie(make_shared<Point>(x - 2, y + 1), side, false));
    path.push_back(addOrDie(make_shared<Point>(x - 2, y - 1), side, false));

    /// to the top
    path.push_back(addOrDie(make_shared<Point>(x + 1, y + 2), side, false));
    path.push_back(addOrDie(make_shared<Point>(x - 1, y + 2), side, false));

    /// to the bottom
    path.push_back(addOrDie(make_shared<Point>(x + 1, y - 2), side, false));
    path.push_back(addOrDie(make_shared<Point>(x - 1, y - 2), side, false));

    return path;
}

list<shared_ptr<Point>> PathSystem::buildRookPath(
    const shared_ptr<Figure>& figure) const
{
    BUILD_PATH_INTRO;

    /// to the right
    for (int i = x + 1; i < 8; ++i) {
        auto p = addOrDie(make_shared<Point>(i, y), side, false);
        if (!p)
            break;
        path.push_back(p);
        auto f = at(p);
        if (f)
            break; // we cannot move through figures
    }

    /// to the left
    for (int i = x - 1; i >= 0; --i) {
        auto p = addOrDie(make_shared<Point>(i, y), side, false);
        if (!p)
            break;
        path.push_back(p);
        auto f = at(p);
        if (f)
            break; // we cannot move through figures
    }

    /// to the top
    for (int i = y + 1; i < 8; ++i) {
        auto p = addOrDie(make_shared<Point>(x, i), side, false);
        if (!p)
            break;
        path.push_back(p);
        auto f = at(p);
        if (f)
            break; // we cannot move through figures
    }

    /// to the bottom
    for (int i = y - 1; i >= 0; --i) {
        auto p = addOrDie(make_shared<Point>(x, i), side, false);
        if (!p)
            break;
        path.push_back(p);
        auto f = at(p);
        if (f)
            break; // we cannot move through figures
    }

    /// check castling

    auto king = side == FigurePlayer::Whites ? at(make_shared<Point>(4, 0))
                                             : at(make_shared<Point>(4, 7));

    if (checkCastling(figure, king))
        path.push_back(make_shared<Point>(king->getX(), y));

    // return
    return path;
}

list<shared_ptr<Point>> PathSystem::buildBishopPath(
    const shared_ptr<Figure>& figure) const
{
    BUILD_PATH_INTRO;

    bool good = true;
    /// to the right-top
    for (int i = x + 1, j = y + 1; j < 8 && i < 8 && good; ++i, ++j) {
        auto p = addOrDie(make_shared<Point>(i, j), side, false);
        path.push_back(p);
        good = !!p ? !at(p) : false; // we cannot move through figures
    }

    good = true;
    /// to the left-top
    for (int i = x - 1, j = y + 1; j < 8 && i >= 0 && good; --i, ++j) {
        auto p = addOrDie(make_shared<Point>(i, j), side, false);
        path.push_back(p);
        good = !!p ? !at(p) : false; // we cannot move through figures
    }

    good = true;
    /// to the right-bottom
    for (int i = x + 1, j = y - 1; j >= 0 && i < 8 && good; ++i, --j) {
        auto p = addOrDie(make_shared<Point>(i, j), side, false);
        path.push_back(p);
        good = !!p ? !at(p) : false; // we cannot move through figures
    }

    good = true;
    /// to the left-bottom
    for (int i = x - 1, j = y - 1; j >= 0 && i >= 0 && good; --i, --j) {
        auto p = addOrDie(make_shared<Point>(i, j), side, false);
        path.push_back(p);
        good = !!p ? !at(p) : false; // we cannot move through figures
    }

    return path;
}

list<shared_ptr<Point>> PathSystem::buildKingPath(
    const shared_ptr<Figure>& figure) const
{
    BUILD_PATH_INTRO;

    path.push_back(addOrDie(make_shared<Point>(x + 1, y), side, false));
    path.push_back(addOrDie(make_shared<Point>(x - 1, y), side, false));

    path.push_back(addOrDie(make_shared<Point>(x + 1, y + 1), side, false));
    path.push_back(addOrDie(make_shared<Point>(x - 1, y + 1), side, false));
    path.push_back(addOrDie(make_shared<Point>(x - 1, y - 1), side, false));
    path.push_back(addOrDie(make_shared<Point>(x + 1, y - 1), side, false));

    path.push_back(addOrDie(make_shared<Point>(x, y + 1), side, false));
    path.push_back(addOrDie(make_shared<Point>(x, y - 1), side, false));

    auto rook1 = side == FigurePlayer::Whites ? at(make_shared<Point>(0, 0))
                                              : at(make_shared<Point>(0, 7));
    auto rook2 = side == FigurePlayer::Whites ? at(make_shared<Point>(7, 0))
                                              : at(make_shared<Point>(7, 7));

    if (rook1 && checkCastling(figure, rook1))
        path.push_back(make_shared<Point>(x - 2, y));

    if (rook2 && checkCastling(figure, rook2))
        path.push_back(make_shared<Point>(x + 2, y));

    return path;
}

bool PathSystem::checkCastling(const shared_ptr<Figure>& one,
    const shared_ptr<Figure>& two) const
{
    if (!one || !one->isReadyForCastling() || !two || !two->isReadyForCastling() /// not ready for castling
        || one->getPlayer() != two->getPlayer() || /// different sides
        (*one == *two && *one->getPoint() == *two->getPoint())) /// two same figures
        return false; /// no castling
    shared_ptr<Figure> rook;
    shared_ptr<Figure> king;

    if (one->isRook() && two->isKing()) {
        rook = one;
        king = two;
    } else if (two->isRook() && one->isKing()) {
        king = one;
        rook = two;
    } else
        return false; /// no rook & king - no castling

    bool castlingPathClear = true;

    auto kingX = king->getX();
    auto rookX = rook->getX();

    auto point = make_shared<Point>(0, king->getY());
    for (auto i = min(kingX + 1, rookX + 1); i < max(kingX, rookX); ++i) {
        point->setX(i);
        if (!at(point))
            continue;
        castlingPathClear = false;
        /// tmp is an obstacle for castling
        break;
    }

    return castlingPathClear;
}

shared_ptr<Figure> PathSystem::at(const shared_ptr<Point>& point) const
{
    if (!point || !point->inBounds())
        return nullptr;

    for (const auto& item : board)
        if (*(item->getPoint()) == *point)
            return item;
    return nullptr;
}

const list<std::shared_ptr<Figure>>& PathSystem::getBoard() const
{
    return board;
}

void PathSystem::setBoard(const list<std::shared_ptr<Figure>>& list)
{
    board = list;
}

list<shared_ptr<Point>> PathSystem::checkForAnyMovement(
    const shared_ptr<Figure>& from) const
{
    if (!from)
        return {};
    return buildPath(from);
}

bool PathSystem::checkForMovement(const shared_ptr<Figure>& figure,
    const shared_ptr<Point>& to) const
{
    if (!figure || !to)
        return false;

    auto possibleMoves = checkForAnyMovement(figure);

    // after we got path we perform actual movement if final point is on path
    bool finalOnPath = false;
    for (const auto& i : possibleMoves)
        if (*to == *i) {
            finalOnPath = true;
            break;
        }
    if (!finalOnPath)
        return false;

    auto possibleFigure = at(to);
    if (possibleFigure && possibleFigure->isKing()) // king cannot be killed
        return false;

    auto moves = getListOfAvailableMoves(figure->getPlayer());

    for (const auto& i : moves)
        if (*i.first == *figure)
            if (*i.second == *to)
                return true;

    return false;
}

multimap<shared_ptr<Figure>, shared_ptr<Point>>
PathSystem::getRawListOfMoves(FigurePlayer side) const
{
    // ally & enemy figures
    set<shared_ptr<Figure>> allies, enemies;
    shared_ptr<Figure> ourKing;

    for (const auto& i : board) {
        if (!i->isAlive())
            continue;
        if (i->getPlayer() == side) {
            allies.insert(i);
            if (i->isKing())
                ourKing = i;
        } else
            enemies.insert(i);
    }

    if (!ourKing) { // Are we in testing mode?
        throw runtime_error("two kings must be at board!");
    }

    // build moves map
    set<shared_ptr<Point>> interceptionPoint;
    multimap<shared_ptr<Figure>, shared_ptr<Point>> allyForces;

    // trace back method
    auto traceBack = [&](
                         const shared_ptr<Figure>& figure) -> list<shared_ptr<Point>> {
        auto destinyPoint = figure->getPoint();
        if (figure->isKnight())
            return { destinyPoint }; // we cannot block knight's checkmate

        long dx = (long)ourKing->getPoint()->getX() - destinyPoint->getX(),
             dy = (long)ourKing->getPoint()->getY() - destinyPoint->getY();

        int enemyAboveKing = dy < 0 ? 1 : dy == 0 ? 0 : -1;
        int enemyOnTheRight = dx < 0 ? 1 : dx == 0 ? 0 : -1;

        list<shared_ptr<Point>> tracedPath;
        shared_ptr<Point> point = make_shared<Point>(ourKing->getPoint()->getX(),
            ourKing->getPoint()->getY());
        while (*point != *destinyPoint) {
            point->shift(enemyOnTheRight, enemyAboveKing);
            tracedPath.push_back(make_shared<Point>(*point));
        }
        return tracedPath;
    };

    for (const auto& i : enemies)
        // look for our king's position
        for (const auto& pos : buildPath(i))
            if (*pos == *ourKing->getPoint()) {
                // trace back path from king to attacker
                auto tracedPoints = traceBack(i);
                interceptionPoint.insert(tracedPoints.begin(), tracedPoints.end());
                break;
            }

    for (const auto& i : allies) {
        auto path = buildPath(i); // look for protection

        for (const auto& pos : path) {
            bool isInWanted = interceptionPoint
                                  .empty(); // if no checkmate set - we want all the moves
            // look for 'pos' in interceptionPoints
            if (!isInWanted) {
                for (const auto& want : interceptionPoint) {
                    isInWanted = i->isKing() ? (!!at(want)) : (*want == *pos);
                    if (isInWanted)
                        break;
                }
            }

            if (isInWanted)
                allyForces.insert({ i, pos });
        }
    }
    return allyForces;
}

multimap<shared_ptr<Figure>, shared_ptr<Point>>
PathSystem::getListOfAvailableMoves(FigurePlayer side) const
{
    /// [START ## CHECK 1 ## START] => produces list of figures that can protect
    /// king
    /// --------------------------------###########----------------------------------
    /// first of all we build huge list of points where our figures can move
    /// next, we build huge list of turns our enemy can make
    /// then we have to look for enemy turns that touch our king and place all the
    /// way from king to them figures to the list A
    /// after that we look back at ally points list and choose only those that are
    /// in list A + those
    /// that can capture checking figure

    /// So, we need following lists: allyPoints, enemyPoints,
    /// checkmateEnemyPoints, interceptedPoints + capturingCheckerPoints

    /// From last two lists we form a special list of ally figures that can
    /// protect our king
    /// --------------------------------###########----------------------------------
    /// [END ## CHECK 1 ## END]

    /// butt then

    /// we have to build a whole new chessboard with already performed movement
    /// individually for each ally figure from
    /// previously made list
    /// for each case we have to run modified CHECK_1 that returns boolean that
    /// helps us see if this particular movement
    /// can potentially open way for some enemy entry to checkmate our fellow
    /// king.

    /// CHECK_1 part
    auto oppositeSide = side == Whites ? Blacks : Whites;
    auto allyForces = getRawListOfMoves(side);

    // next is simulation of one turn

    auto king = getKing(side);
    if (!king)
        throw runtime_error("two kings must be at board!");

    multimap<shared_ptr<Figure>, shared_ptr<Point>> filteredAllies,
        doubleFilteredAllies;

    for (const auto& i : allyForces) {
        const auto& figure = i.first;
        const auto& spot = i.second;
        const auto figurePos = make_shared<Point>(*figure->getPoint());

        auto possibleFigure = at(spot);
        if (possibleFigure)
            possibleFigure->isCapturedBy(figure);

        figure->getPoint()->setX(spot->getX());
        figure->getPoint()->setY(spot->getY());

        bool futureBlock = false;
        auto enemyTurns = getRawListOfMoves(oppositeSide);

        for (const auto& enemyPair : enemyTurns) {
            auto enemyFigure = enemyPair.first;
            auto enemyTurn = enemyPair.second;
            if (*enemyTurn == *king->getPoint()) {
                futureBlock = true;
                break;
            }
        }

        if (!futureBlock)
            filteredAllies.insert({ figure, spot });

        if (possibleFigure)
            possibleFigure->revive();
        figure->getPoint()->setX(figurePos->getX());
        figure->getPoint()->setY(figurePos->getY());
    }

    const auto leftCastlePoint = Point(king->getX() - 2, king->getY());
    const auto rightCastlePoint = Point(king->getX() + 2, king->getY());
    bool kingCanCastleLeft = false, kingCanCastleRight = false;

    for (const auto& i : filteredAllies)
        if (*i.first == *king && *(i.first->getPoint()) == *king->getPoint()) {
            if (*i.second == rightCastlePoint)
                kingCanCastleRight = true;
            else if (*i.second == leftCastlePoint)
                kingCanCastleLeft = true;
        }

    for (const auto& i : filteredAllies) {
        const auto& figure = i.first;
        const bool kingCanCastle = figure->getX() < king->getX() ? kingCanCastleLeft : kingCanCastleRight;
        const bool isKingNow = *i.first == *king && *(i.first->getPoint()) == *(king->getPoint());
        const bool toInsert = isKingNow || (figure->isReadyForCastling() ? checkCastling(figure, king) && kingCanCastle : true);
        if (i.second && toInsert)
            doubleFilteredAllies.insert(i);
    }

    return doubleFilteredAllies;
}

shared_ptr<Figure> PathSystem::getKing(FigurePlayer side) const
{
    for (const auto& i : board)
        if (i->getPlayer() == side)
            if (i->isKing())
                return i;
    return nullptr;
}
