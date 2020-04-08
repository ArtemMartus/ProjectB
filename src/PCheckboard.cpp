//
// Created by Artem Martus on 06.04.2020.
//

#define BUILD_PATH_INTRO if (!figure) throw invalid_argument("Cannot build path for null pointer");\
                        list<PPoint *> path;\
                        int x = figure->getPoint()->getX(), y = figure->getPoint()->getY();\
                        auto side = figure->getPlayer();


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

PFigure *PCheckboard::at(PPoint *point) {
	if (!point || !point->inBounds()) return nullptr;

	for (const auto item: m_board)
		if (item->getPoint()->isEquals(point))
			return item;
	return nullptr;
}

bool PCheckboard::prepareMove(PPoint *from, PPoint *to) {
	// check initial arguments
	if (!from || !to) throw invalid_argument("Cannot move null pointer to another position");
	// recheck checkbox for ally figure

	auto figure = at(from);
	if (!figure)
		return false;
	// make aliases for readability
	const auto side = figure->getPlayer();
	const auto type = figure->getType();

	list<PPoint *> possibleMoves = buildPath(figure);

	// after we got path we perform actual movement if final point is on path
	bool finalOnPath = false;
	for (auto i: possibleMoves)
		if (to->isEquals(i)) { /// cannot use find because we need isEquals method
			finalOnPath = true;
			break;
		}
	if (!finalOnPath) return false;

	performMovement(figure, to);

	// after the movement we update board figures if needed and check special morphs for pawns
	if (type == FigureType::Pawn && !m_deadFigures.empty()) {
		int endY = side == FigurePlayer::Whites ? 7 : 0;
		if (to->getY() == endY) {
			PFigure *undead = nullptr;
			int temp = -1; // get the most valuable from dead list
			for (auto i: m_deadFigures) {
				if (i->getPlayer() != side) continue;
				if (i->getType() > temp) {
					undead = i;
					temp = i->getType();
				}
			}
			if (!undead) throw invalid_argument("couldn't choose from dead figures");

			undead->revive();
			undead->moved();
			undead->getPoint()->setX(to->getX());
			undead->getPoint()->setY(to->getY());
			m_deadFigures.remove(undead);
			m_board.push_back(undead);
			figure->kill(undead);
			m_deadFigures.push_back(figure);
			m_board.remove(figure);

		}
	}
	// and free up used memory
	for (auto item: possibleMoves)
		delete item;

	return true;
}

list<PFigure *> PCheckboard::getAliveFigures() const {
	return m_board;
}

bool PCheckboard::onePlayerLeft() const {
	auto side = m_board.front()->getPlayer();
	for (auto item: m_board)
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
	if (!m_board.empty()) {
		for (auto item: m_board)
			delete item;
		m_board.clear();
	}

	if (!m_deadFigures.empty()) {
		for (auto item: m_deadFigures)
			delete item;
		m_deadFigures.clear();
	}
}


// save-load block

PCheckboard::PCheckboard(list<PFigure *> figures) {
	for (const auto item: figures) {
		if (item->isAlive())
			m_board.push_back(item);
		else
			m_deadFigures.push_back(item);
	}
}

list<PFigure *> PCheckboard::getAllFigures() const {
	list<PFigure *> out;
	out.insert(out.end(), m_board.begin(), m_board.end());
	out.insert(out.end(), m_deadFigures.begin(), m_deadFigures.end());

	return out;
}

list<PPoint *> PCheckboard::buildPath(PFigure *figure) {
	if (!figure) throw invalid_argument("Cannot build path for null pointer");

	list<PPoint *> possibleMoves;

	// build possible path for different figure types
	if (figure->isPawn())
		possibleMoves.splice(possibleMoves.end(), buildPawnPath(figure));
	if (figure->isRook() || figure->isQueen())
		possibleMoves.splice(possibleMoves.end(), buildRookPath(figure));

	if (figure->isKnight())
		possibleMoves.splice(possibleMoves.end(), buildKnightPath(figure));

	if (figure->isBishop() || figure->isQueen())
		possibleMoves.splice(possibleMoves.end(), buildBishopPath(figure));

	if (figure->isKing())
		possibleMoves.splice(possibleMoves.end(), buildKingPath(figure));

	possibleMoves.remove(nullptr); /// there may be so many of them, not really want to break any iterators
	return possibleMoves;
}


PPoint *PCheckboard::addOrDie(PPoint *p, FigurePlayer side, bool pawnMode) {
	if (!p) throw invalid_argument("cannot addOrDie null pointer");

	auto fig = at(p);
	bool differentSides = fig && fig->getPlayer() != side;
	// pawns can capture on diagonals but not horizontally
	bool allowNext = p->inBounds() && (pawnMode ? (fig && differentSides) : (!fig || differentSides));

	if (allowNext)
		return p;
	else
		delete p;
	return nullptr;
}

list<PPoint *> PCheckboard::buildPawnPath(PFigure *figure) {
	BUILD_PATH_INTRO
	auto pawnY = side == FigurePlayer::Whites ? 1 : -1;

	auto p = addOrDie(new PPoint(x, y + pawnY), side, false);        // cannot attack forward
	if (p && !at(p)) {
		path.push_back(p);
		if (figure->getMovesCount() == 0)
			path.push_back(addOrDie(new PPoint(x, y + 2 * pawnY), side, false));
	}
	else
		delete p;

	path.push_back(addOrDie(new PPoint(x + 1, y + pawnY), side, true));
	path.push_back(addOrDie(new PPoint(x - 1, y + pawnY), side, true));

	return path;
}


list<PPoint *> PCheckboard::buildKnightPath(PFigure *figure) {
	BUILD_PATH_INTRO

	/// to the right
	path.push_back(addOrDie(new PPoint(x + 2, y + 1), side, false));
	path.push_back(addOrDie(new PPoint(x + 2, y - 1), side, false));

	/// to the left
	path.push_back(addOrDie(new PPoint(x - 2, y + 1), side, false));
	path.push_back(addOrDie(new PPoint(x - 2, y - 1), side, false));

	/// to the top
	path.push_back(addOrDie(new PPoint(x + 1, y + 2), side, false));
	path.push_back(addOrDie(new PPoint(x - 1, y + 2), side, false));

	/// to the bottom
	path.push_back(addOrDie(new PPoint(x + 1, y - 2), side, false));
	path.push_back(addOrDie(new PPoint(x - 1, y - 2), side, false));

	return path;
}

list<PPoint *> PCheckboard::buildRookPath(PFigure *figure) {
	BUILD_PATH_INTRO

	/// to the right
	for (int i = x + 1; i < 8; ++i) {
		auto p = addOrDie(new PPoint(i, y), side, false);
		if (!p) break;
		path.push_back(p);
		auto f = at(p);
		if (f) break; // we cannot move through figures
	}

	/// to the left
	for (int i = x - 1; i >= 0; --i) {
		auto p = addOrDie(new PPoint(i, y), side, false);
		if (!p) break;
		path.push_back(p);
		auto f = at(p);
		if (f) break; // we cannot move through figures
	}

	/// to the top
	for (int i = y + 1; i < 8; ++i) {
		auto p = addOrDie(new PPoint(x, i), side, false);
		if (!p) break;
		path.push_back(p);
		auto f = at(p);
		if (f) break; // we cannot move through figures
	}

	/// to the bottom
	for (int i = y - 1; i >= 0; --i) {
		auto p = addOrDie(new PPoint(x, i), side, false);
		if (!p) break;
		path.push_back(p);
		auto f = at(p);
		if (f) break; // we cannot move through figures
	}

	/// check castling

	auto kingSpot = side == FigurePlayer::Whites ? new PPoint(4, 0) : new PPoint(4, 7);

	if (checkCastling(figure, at(kingSpot)))
		path.push_back(kingSpot);
	else
		delete kingSpot;

	// return
	return path;
}

list<PPoint *> PCheckboard::buildBishopPath(PFigure *figure) {
	BUILD_PATH_INTRO

	bool good = true;
	/// to the right-top
	for (int i = x + 1, j = y + 1; j < 8 && i < 8 && good; ++i, ++j) {
		auto p = addOrDie(new PPoint(i, j), side, false);
		path.push_back(p);
		good = p != nullptr;
		if (good) {
			auto f = at(p);
			if (f) good = false; // we cannot move through figures
		}
	}

	good = true;
	/// to the left-top
	for (int i = x - 1, j = y + 1; j < 8 && i >= 0 && good; --i, ++j) {
		auto p = addOrDie(new PPoint(i, j), side, false);
		path.push_back(p);
		good = p != nullptr;
		if (good) {
			auto f = at(p);
			if (f) good = false; // we cannot move through figures
		}
	}

	good = true;
	/// to the right-bottom
	for (int i = x + 1, j = y - 1; j >= 0 && i < 8 && good; ++i, --j) {
		auto p = addOrDie(new PPoint(i, j), side, false);
		path.push_back(p);
		good = p != nullptr;
		if (good) {
			auto f = at(p);
			if (f) good = false; // we cannot move through figures
		}
	}


	good = true;
	/// to the left-bottom
	for (int i = x - 1, j = y - 1; j >= 0 && i >= 0 && good; --i, --j) {
		auto p = addOrDie(new PPoint(i, j), side, false);
		path.push_back(p);
		good = p != nullptr;
		if (good) {
			auto f = at(p);
			if (f) good = false; // we cannot move through figures
		}
	}

	return path;
}

list<PPoint *> PCheckboard::buildKingPath(PFigure *figure) {
	BUILD_PATH_INTRO

	path.push_back(addOrDie(new PPoint(x + 1, y), side, false));
	path.push_back(addOrDie(new PPoint(x - 1, y), side, false));

	path.push_back(addOrDie(new PPoint(x + 1, y + 1), side, false));
	path.push_back(addOrDie(new PPoint(x - 1, y + 1), side, false));
	path.push_back(addOrDie(new PPoint(x - 1, y - 1), side, false));
	path.push_back(addOrDie(new PPoint(x + 1, y - 1), side, false));

	path.push_back(addOrDie(new PPoint(x, y + 1), side, false));
	path.push_back(addOrDie(new PPoint(x, y - 1), side, false));

	auto rook1Spot = side == FigurePlayer::Whites ? new PPoint(0, 0) : new PPoint(0, 7);
	auto rook2Spot = side == FigurePlayer::Whites ? new PPoint(7, 0) : new PPoint(7, 7);

	if (checkCastling(figure, at(rook1Spot)))
		path.push_back(rook1Spot);
	else
		delete rook1Spot;

	if (checkCastling(figure, at(rook2Spot)))
		path.push_back(rook2Spot);
	else
		delete rook2Spot;

	return path;
}

void PCheckboard::setTurn(bool w) {
	whitesTurn = w;
}

bool PCheckboard::getWhitesTurn() const {
	return whitesTurn;
}

bool PCheckboard::checkCastling(PFigure *one, PFigure *two) {
	if (!one || !two) throw invalid_argument("Cannot perform castling on null pointers");
	PFigure *rook;
	PFigure *king;

	if (!one->readyForCastling() || !two->readyForCastling())
		return false;  /// no castling

	if (one->getPlayer() != two->getPlayer())
		return false; /// different sides

	if (one->isRook() && two->isKing()) {
		rook = one;
		king = two;
	} else if (two->isRook() && one->isKing()) {
		king = one;
		rook = two;
	} else
		return false; /// no rooks no castling


	bool castlingPathClear = true;

	auto kingX = king->getPoint()->getX();
	auto rookX = rook->getPoint()->getX();

	auto point = new PPoint(0, king->getPoint()->getY());
	for (auto i = min(kingX + 1, rookX + 1); i < max(kingX, rookX); ++i) {
		point->setX(i);
		auto tmp = at(point);
		if (!tmp) continue;
		castlingPathClear = false;
		/// tmp is an obstacle for castling
		break;
	}
	delete point;

	return castlingPathClear;
}

void PCheckboard::performMovement(PFigure *figure, PPoint *to) {
	if (!figure || !to) throw invalid_argument("Cannot move null from to pointers");

	auto fromX = figure->getPoint()->getX();

	auto possibleFigure = at(to);

	if (possibleFigure) {
		if (possibleFigure->getPlayer() != figure->getPlayer()) {
			possibleFigure->kill(figure);
			m_board.remove(possibleFigure);
			m_deadFigures.push_back(possibleFigure);

		} else { /// else we castle
			PFigure *king, *rook;
			if (possibleFigure->isKing()) {
				king = possibleFigure;
				rook = figure;
			} else {
				king = figure;
				rook = possibleFigure;
			}

			if (king->getPoint()->getX() < rook->getPoint()->getX()) { /// castle to the left
				king->getPoint()->setX(king->getPoint()->getX() - 2);
				rook->getPoint()->setX(king->getPoint()->getX() + 1);
				rook->moved();
				king->moved();

			} else { /// castle to the right
				figure->getPoint()->setX(fromX + 2);
				possibleFigure->getPoint()->setX(figure->getPoint()->getX() - 1);
				possibleFigure->moved();

			}
		}
	} else {
		figure->getPoint()->setX(to->getX());
		figure->getPoint()->setY(to->getY());
		figure->moved();
	}
}

list<PFigure *> PCheckboard::buildSide(FigurePlayer side) {
	list<PFigure *> figures;    // chessboard consists of 8x8 squares

	figures.splice(figures.end(), buildPawns(side));    /// 8 pawns
	figures.splice(figures.end(), buildRooks(side));    /// 2 rooks
	figures.splice(figures.end(), buildKnights(side));    /// 2 knights
	figures.splice(figures.end(), buildBishops(side));    /// 2 bishops
	figures.push_back(buildQueen(side));    /// one queen
	figures.push_back(buildKing(side)); /// one king

	return figures;
}

list<PFigure *> PCheckboard::buildPawns(FigurePlayer side) {
	list<PFigure *> pawns;
	auto pawnY = side == FigurePlayer::Whites ? 1 : 6;
	for (int i = 0; i < 8; i++)
		pawns.push_back(
				new PFigureImpl(
						new PPoint(i, pawnY),
						FigureType::Pawn, side));

	return pawns;
}

#define BUILD_FIGURES_Y auto y = side == FigurePlayer::Whites ? 7 : 0; // 7 for whites, 0 for blacks

list<PFigure *> PCheckboard::buildRooks(FigurePlayer side) {
	BUILD_FIGURES_Y

	return {
			new PFigureImpl(new PPoint(0, 7 - y), FigureType::Rook, side),
			new PFigureImpl(new PPoint(7, 7 - y), FigureType::Rook, side)
	};
}

list<PFigure *> PCheckboard::buildKnights(FigurePlayer side) {
	BUILD_FIGURES_Y

	return {
			new PFigureImpl(new PPoint(1, 7 - y), FigureType::Knight, side),
			new PFigureImpl(new PPoint(6, 7 - y), FigureType::Knight, side)
	};
}

list<PFigure *> PCheckboard::buildBishops(FigurePlayer side) {
	BUILD_FIGURES_Y

	return {new PFigureImpl(new PPoint(2, 7 - y), FigureType::Bishop, side),
	        new PFigureImpl(new PPoint(5, 7 - y), FigureType::Bishop, side)};
}

PFigure *PCheckboard::buildQueen(FigurePlayer side) {
	BUILD_FIGURES_Y

	return new PFigureImpl(new PPoint(3, 7 - y), FigureType::Queen, side);
}

PFigure *PCheckboard::buildKing(FigurePlayer side) {
	BUILD_FIGURES_Y

	return new PFigureImpl(new PPoint(4, 7 - y), FigureType::King, side);
}

#ifndef NDEBUG

void PCheckboard::addFigure(PFigure *figure) {
	m_board.push_back(figure);
}

void PCheckboard::removeFigure(PFigure *figure) {
	m_board.remove(figure);
	m_deadFigures.remove(figure);
}

#endif