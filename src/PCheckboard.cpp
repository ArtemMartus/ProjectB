//
// Created by Artem Martus on 06.04.2020.
//

#define BUILD_PATH_INTRO \
                        list<shared_ptr<PPoint>> path;\
                        int x = figure.getPoint()->getX(), y = figure.getPoint()->getY();\
                        auto side = figure.getPlayer();


#include "PCheckboard.h"
#include "PFigure.h"
#include "PPoint.h"
#include <list>
#include <stdexcept>

using namespace std;

PCheckboard::PCheckboard() {
	m_board.clear();
}

PCheckboard::~PCheckboard() {
	destroy();
}

shared_ptr<PFigure> PCheckboard::at(const PPoint &point) {
	if (!point.inBounds()) return nullptr;

	for (const auto &item: m_board)
		if (*(item->getPoint()) == point)
			return item;
	return nullptr;
}

bool PCheckboard::prepareMove(const PPoint &from, const PPoint &to) {
	// recheck checkbox for ally figure

	auto figure = at(from);
	if (!figure)
		return false;
	// make aliases for readability
	const auto side = figure->getPlayer();
	const auto type = figure->getType();

	auto possibleMoves = buildPath(*figure);

	// after we got path we perform actual movement if final point is on path
	bool finalOnPath = false;
	for (const auto &i: possibleMoves)
		if (to == *i) {
			finalOnPath = true;
			break;
		}
	if (!finalOnPath) return false;

	performMovement(figure, to);

	// after the movement we update board figures if needed and check special morphs for pawns
	if (type == FigureType::Pawn && !m_deadFigures.empty()) {
		int endY = side == FigurePlayer::Whites ? 7 : 0;
		if (to.getY() == endY) {
			shared_ptr<PFigure> undead;
			int temp = -1; // get the most valuable from dead list
			for (const auto &i: m_deadFigures) {
				if (i->getPlayer() != side) continue;
				if (i->getType() > temp) {
					undead = i;
					temp = i->getType();
				}
			}
			if (!undead) throw invalid_argument("couldn't choose from dead figures");

			undead->revive();
			undead->moved();
			undead->getPoint()->setX(to.getX());
			undead->getPoint()->setY(to.getY());
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
	auto blacks = buildSide(FigurePlayer::Blacks);
	auto whites = buildSide(FigurePlayer::Whites);

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
}

list<shared_ptr<PFigure>> PCheckboard::getAllFigures() const {
	list<shared_ptr<PFigure>> out;
	out.insert(out.end(), m_board.begin(), m_board.end());
	out.insert(out.end(), m_deadFigures.begin(), m_deadFigures.end());

	return out;
}

list<shared_ptr<PPoint>> PCheckboard::buildPath(const PFigure &figure) {
	list<shared_ptr<PPoint>> rawMoves;

	// build possible path for different figure types
	if (figure.isPawn())
		rawMoves.splice(rawMoves.end(), buildPawnPath(figure));

	if (figure.isRook() || figure.isQueen())
		rawMoves.splice(rawMoves.end(), buildRookPath(figure));

	if (figure.isKnight())
		rawMoves.splice(rawMoves.end(), buildKnightPath(figure));

	if (figure.isBishop() || figure.isQueen())
		rawMoves.splice(rawMoves.end(), buildBishopPath(figure));

	if (figure.isKing()) {
		rawMoves.splice(rawMoves.end(), buildKingPath(figure));
		/// after that we perform search on enemies move abilities and
		/// exclude those intercepting with our king's path points for rawMoves
		/// TODO: exclude dangerous paths
	}

	/// there may be so many of them, not really want to break any iterators
	list<shared_ptr<PPoint>> possibleMoves;
	for (const auto& i: rawMoves)
		if (i)
			possibleMoves.push_back(i);

	return possibleMoves;
}


shared_ptr<PPoint> PCheckboard::addOrDie(const PPoint &p, FigurePlayer side, bool pawnMode) {
	auto fig = at(p);
	bool differentSides = fig && fig->getPlayer() != side;
	// pawns can capture on diagonals but not horizontally
	bool allowNext = p.inBounds() && (pawnMode ? (fig && differentSides) : (!fig || differentSides));

	if (allowNext)
		return make_shared<PPoint>(p);

	return nullptr;
}

list<shared_ptr<PPoint>> PCheckboard::buildPawnPath(const PFigure &figure) {
	BUILD_PATH_INTRO
	auto pawnY = side == FigurePlayer::Whites ? 1 : -1;

	auto p = addOrDie(PPoint(x, y + pawnY), side, false);        // cannot attack forward
	if (p && !at(*p)) {
		path.push_back(p);
		if (figure.getMovesCount() == 0)
			path.push_back(addOrDie(PPoint(x, y + 2 * pawnY), side, false));
	}

	path.push_back(addOrDie(PPoint(x + 1, y + pawnY), side, true));
	path.push_back(addOrDie(PPoint(x - 1, y + pawnY), side, true));

	return path;
}


list<shared_ptr<PPoint>> PCheckboard::buildKnightPath(const PFigure &figure) {
	BUILD_PATH_INTRO

	/// to the right
	path.push_back(addOrDie(PPoint(x + 2, y + 1), side, false));
	path.push_back(addOrDie(PPoint(x + 2, y - 1), side, false));

	/// to the left
	path.push_back(addOrDie(PPoint(x - 2, y + 1), side, false));
	path.push_back(addOrDie(PPoint(x - 2, y - 1), side, false));

	/// to the top
	path.push_back(addOrDie(PPoint(x + 1, y + 2), side, false));
	path.push_back(addOrDie(PPoint(x - 1, y + 2), side, false));

	/// to the bottom
	path.push_back(addOrDie(PPoint(x + 1, y - 2), side, false));
	path.push_back(addOrDie(PPoint(x - 1, y - 2), side, false));

	return path;
}

list<shared_ptr<PPoint>> PCheckboard::buildRookPath(const PFigure &figure) {
	BUILD_PATH_INTRO

	/// to the right
	for (int i = x + 1; i < 8; ++i) {
		auto p = addOrDie(PPoint(i, y), side, false);
		if (!p) break;
		path.push_back(p);
		auto f = at(*p);
		if (f) break; // we cannot move through figures
	}

	/// to the left
	for (int i = x - 1; i >= 0; --i) {
		auto p = addOrDie(PPoint(i, y), side, false);
		if (!p) break;
		path.push_back(p);
		auto f = at(*p);
		if (f) break; // we cannot move through figures
	}

	/// to the top
	for (int i = y + 1; i < 8; ++i) {
		auto p = addOrDie(PPoint(x, i), side, false);
		if (!p) break;
		path.push_back(p);
		auto f = at(*p);
		if (f) break; // we cannot move through figures
	}

	/// to the bottom
	for (int i = y - 1; i >= 0; --i) {
		auto p = addOrDie(PPoint(x, i), side, false);
		if (!p) break;
		path.push_back(p);
		auto f = at(*p);
		if (f) break; // we cannot move through figures
	}

	/// check castling

	auto kingSpot = side == FigurePlayer::Whites ? PPoint(4, 0) : PPoint(4, 7);

	if (checkCastling(figure, *at(kingSpot)))
		path.push_back(make_shared<PPoint>(kingSpot));

	// return
	return path;
}

list<shared_ptr<PPoint>> PCheckboard::buildBishopPath(const PFigure &figure) {
	BUILD_PATH_INTRO

	bool good = true;
	/// to the right-top
	for (int i = x + 1, j = y + 1; j < 8 && i < 8 && good; ++i, ++j) {
		auto p = addOrDie(PPoint(i, j), side, false);
		path.push_back(p);
		good = (p != nullptr) ?
		       (at(*p) != nullptr)  // we cannot move through figures
		                      : false;
	}

	good = true;
	/// to the left-top
	for (int i = x - 1, j = y + 1; j < 8 && i >= 0 && good; --i, ++j) {
		auto p = addOrDie(PPoint(i, j), side, false);
		path.push_back(p);
		good = (p != nullptr) ?
		       (at(*p) != nullptr)  // we cannot move through figures
		                      : false;
	}

	good = true;
	/// to the right-bottom
	for (int i = x + 1, j = y - 1; j >= 0 && i < 8 && good; ++i, --j) {
		auto p = addOrDie(PPoint(i, j), side, false);
		path.push_back(p);
		good = (p != nullptr) ?
		       (at(*p) != nullptr)  // we cannot move through figures
		                      : false;
	}


	good = true;
	/// to the left-bottom
	for (int i = x - 1, j = y - 1; j >= 0 && i >= 0 && good; --i, --j) {
		auto p = addOrDie(PPoint(i, j), side, false);
		path.push_back(p);
		good = (p != nullptr) ?
		       (at(*p) != nullptr)  // we cannot move through figures
		                      : false;
	}

	return path;
}

list<shared_ptr<PPoint>> PCheckboard::buildKingPath(const PFigure &figure) {
	BUILD_PATH_INTRO

	path.push_back(addOrDie(PPoint(x + 1, y), side, false));
	path.push_back(addOrDie(PPoint(x - 1, y), side, false));

	path.push_back(addOrDie(PPoint(x + 1, y + 1), side, false));
	path.push_back(addOrDie(PPoint(x - 1, y + 1), side, false));
	path.push_back(addOrDie(PPoint(x - 1, y - 1), side, false));
	path.push_back(addOrDie(PPoint(x + 1, y - 1), side, false));

	path.push_back(addOrDie(PPoint(x, y + 1), side, false));
	path.push_back(addOrDie(PPoint(x, y - 1), side, false));

	auto rook1 = side == FigurePlayer::Whites ? at(PPoint(0, 0)) : at(PPoint(0, 7));
	auto rook2 = side == FigurePlayer::Whites ? at(PPoint(7, 0)) : at(PPoint(7, 7));

	if (rook1 && checkCastling(figure, *rook1))
		path.push_back(make_shared<PPoint>(*rook1->getPoint()));

	if (rook2 && checkCastling(figure, *rook2))
		path.push_back(make_shared<PPoint>(*rook2->getPoint()));

	return path;
}

void PCheckboard::setTurn(bool w) {
	whitesTurn = w;
}

bool PCheckboard::getWhitesTurn() const {
	return whitesTurn;
}

bool PCheckboard::checkCastling(const PFigure &one, const PFigure &two) {
	const PFigure *rook;
	const PFigure *king;

	if (!one.readyForCastling() || !two.readyForCastling()
	    || one.getPlayer() != two.getPlayer())/// different sides
		return false;  /// no castling


	if (one.isRook() && two.isKing()) {
		rook = &one;
		king = &two;
	} else if (two.isRook() && one.isKing()) {
		king = &one;
		rook = &two;
	} else
		return false; /// no rook & king - no castling


	bool castlingPathClear = true;

	auto kingX = king->getPoint()->getX();
	auto rookX = rook->getPoint()->getX();

	auto point = PPoint(0, king->getPoint()->getY());
	for (auto i = min(kingX + 1, rookX + 1); i < max(kingX, rookX); ++i) {
		point.setX(i);
		if (!at(point)) continue;
		castlingPathClear = false;
		/// tmp is an obstacle for castling
		break;
	}

	return castlingPathClear;
}

void PCheckboard::performMovement(const shared_ptr<PFigure> &figure, const PPoint &to) {
	if (!figure) throw invalid_argument("Cannot perform movement on nullptr");

	auto possibleFigure = at(to);

	if (possibleFigure) {
		if (possibleFigure->getPlayer() != figure->getPlayer()) {
			possibleFigure->capture(figure);
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
		}
	} else {
		figure->getPoint()->setX(to.getX());
		figure->getPoint()->setY(to.getY());
		figure->moved();
	}
}

list<shared_ptr<PFigure>> PCheckboard::buildSide(FigurePlayer side) {
	list<shared_ptr<PFigure>> figures;    // chessboard consists of 8x8 squares

	figures.splice(figures.end(), buildPawns(side));    /// 8 pawns
	figures.splice(figures.end(), buildRooks(side));    /// 2 rooks
	figures.splice(figures.end(), buildKnights(side));    /// 2 knights
	figures.splice(figures.end(), buildBishops(side));    /// 2 bishops
	figures.push_back(buildQueen(side));    /// one queen
	figures.push_back(buildKing(side)); /// one king

	return figures;
}

list<shared_ptr<PFigure>> PCheckboard::buildPawns(FigurePlayer side) {
	list<shared_ptr<PFigure>> pawns;
	auto pawnY = side == FigurePlayer::Whites ? 1 : 6;
	for (int i = 0; i < 8; i++)
		pawns.push_back(
				make_shared<PFigure>(
						PPoint(i, pawnY),
						FigureType::Pawn, side));

	return pawns;
}

#define BUILD_FIGURES_Y auto y = side == FigurePlayer::Whites ? 7 : 0; // 7 for whites, 0 for blacks

list<shared_ptr<PFigure>> PCheckboard::buildRooks(FigurePlayer side) {
	BUILD_FIGURES_Y

	return {
			make_shared<PFigure>(PPoint(0, 7 - y), FigureType::Rook, side),
			make_shared<PFigure>(PPoint(7, 7 - y), FigureType::Rook, side)
	};
}

list<shared_ptr<PFigure>> PCheckboard::buildKnights(FigurePlayer side) {
	BUILD_FIGURES_Y

	return {
			make_shared<PFigure>(PPoint(1, 7 - y), FigureType::Knight, side),
			make_shared<PFigure>(PPoint(6, 7 - y), FigureType::Knight, side)
	};
}

list<shared_ptr<PFigure>> PCheckboard::buildBishops(FigurePlayer side) {
	BUILD_FIGURES_Y

	return {
			make_shared<PFigure>(PPoint(2, 7 - y), FigureType::Bishop, side),
			make_shared<PFigure>(PPoint(5, 7 - y), FigureType::Bishop, side)};
}

shared_ptr<PFigure> PCheckboard::buildQueen(FigurePlayer side) {
	BUILD_FIGURES_Y

	return make_shared<PFigure>(PPoint(3, 7 - y), FigureType::Queen, side);
}

shared_ptr<PFigure> PCheckboard::buildKing(FigurePlayer side) {
	BUILD_FIGURES_Y

	return make_shared<PFigure>(PPoint(4, 7 - y), FigureType::King, side);
}

#ifndef NDEBUG

void PCheckboard::addFigure(const shared_ptr<PFigure> &figure) {
	m_board.push_back(figure);
}

void PCheckboard::removeFigure(const shared_ptr<PFigure> &figure) {
	m_board.remove(figure);
	m_deadFigures.remove(figure);
}

list<shared_ptr<PFigure>> PCheckboard::getListOfFiguresAvailableForMove(FigurePlayer side) const {
	/// TODO: Implement this method

	/// [START ## CHECK 1 ## START] => produces list of figures that can protect king
	/// first of all we build huge list of points where our figures can move
	/// next, we build huge list of turns our enemy can make
	/// then we have to look for enemy turns that touch our king and place all the way from king to them figures to the list A
	/// after that we look back at ally points list and choose only those that are in list A + those
	/// that can capture checking figure

	/// So, we need following lists: allyPoints, enemyPoints, checkmateEnemyPoints, interceptedPoints + capturingCheckerPoints

	/// From last two lists we form a special list of ally figures that can protect our king
	/// [END ## CHECK 1 ## END]

	/// butt then

	/// we have to build a whole new chessboard with already performed movement individually for each ally figure from
	/// previously made list
	/// for each case we have to run modified CHECK_1 that returns boolean that helps us see if this particular movement
	/// can potentially open way for some enemy entry to checkmate our fellow king.

	return list<shared_ptr<PFigure>>();
}

#endif