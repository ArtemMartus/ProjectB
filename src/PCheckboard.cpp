//
// Created by Artem Martus on 06.04.2020.
//

#define BUILD_PATH_INTRO if (!figure) throw invalid_argument("Cannot build path for null pointer");\
                        list<PPoint *> path;\
                        int x = figure->getPoint()->getX(), y = figure->getPoint()->getY();\
                        auto side = figure->getPlayer(); \
                        auto addOrDie = [&path, side, this](PPoint *p, bool pawnMode = false) {\
                            if (canGo(p) && allowsNext(p, side, pawnMode)) {\
                                path.push_back(p);\
                                return true;\
                            } else\
                                delete p;\
                            return false;\
                        };\



#include "PCheckboard.h"
#include "PFigure.h"
#include "PPoint.h"
#include <list>
#include <stdexcept>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#define arc4random_uniform(x) rand()%x

#elif defined (__linux__)

/// ubuntu apt install libbsd-dev
#include <bsd/stdlib.h>

#endif


using namespace std;

PCheckboard::PCheckboard() {
	initialize();
}

PCheckboard::~PCheckboard() {
	destroy();
}

PFigure *PCheckboard::at(PPoint *point) {
	if (!point) throw std::invalid_argument("Cannot find any figure at null point");
	if (point->getX() < 0 || point->getX() > 7) return nullptr;
	if (point->getY() < 0 || point->getY() > 7) return nullptr;

	for (const auto item: m_board)
		if (item->getPoint()->isEquals(point))
			return item;
	return nullptr;
}

bool PCheckboard::move(PPoint *from, PPoint *to) {
	// check initial arguments
	if (!from || !to) throw std::invalid_argument("Cannot move null pointer to another position");
	// recheck checkbox for ally figure

	// check point bounds
	if (!canGo(to) || !canGo(from))
		return false;

	auto figure = at(from);
	if (!figure)
		return false;
	// make aliases for readability
	auto possibleFigure = at(to);
	const auto side = figure->getPlayer();
	const auto type = figure->getType();

	// check if both points have different player figures
	if (possibleFigure && possibleFigure->getPlayer() == side) return false;

	list<PPoint *> possibleMoves = buildPath(figure);

	// after we got path we perform actual movement if final point is on path
	bool finalOnPath = false;
	for (auto i: possibleMoves)
		if (to->isEquals(i)) {
			finalOnPath = true;
			break;
		}
	if (!finalOnPath) return false;

	figure->getPoint()->setX(to->getX());
	figure->getPoint()->setY(to->getY());

	if (possibleFigure) {
		possibleFigure->kill(figure);
		m_board.remove(possibleFigure);
		m_deadFigures.push_back(possibleFigure);
	}

	// after the movement we update board figures if needed and check special morphs for pawns
	if (type == FigureType::Pawn && !m_deadFigures.empty()) {
		int endY = side == FigurePlayer::Whites ? 7 : 0;
		if (to->getY() == endY) {
			int random_i = arc4random_uniform(m_deadFigures.size());
			PFigure *undead = nullptr;
			int temp = 0;
			for (auto i: m_deadFigures) {
				if (temp == random_i) {
					undead = i;
					break;
				}
				temp++;
			}
			if (!undead) throw invalid_argument("random generator broken");

			undead->revive();
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

	// chessboard consists of 8x8 squares
	auto side = FigurePlayer::Whites;

	/// 8 pawns
	for (int i = 0; i < 8; i++) {
		auto point = new PPoint(i, 1);
		auto pawns = new PFigure(point, FigureType::Pawn, side);
		m_board.push_back(pawns);
	}

	/// 2 rooks
	m_board.push_back(new PFigure(new PPoint(0, 0), FigureType::Rook, side));
	m_board.push_back(new PFigure(new PPoint(7, 0), FigureType::Rook, side));

	/// 2 knights
	m_board.push_back(new PFigure(new PPoint(1, 0), FigureType::Knight, side));
	m_board.push_back(new PFigure(new PPoint(6, 0), FigureType::Knight, side));

	/// 2 bishops
	m_board.push_back(new PFigure(new PPoint(2, 0), FigureType::Bishop, side));
	m_board.push_back(new PFigure(new PPoint(5, 0), FigureType::Bishop, side));

	/// one queen
	m_board.push_back(new PFigure(new PPoint(3, 0), FigureType::Queen, side));

	/// one king
	m_board.push_back(new PFigure(new PPoint(4, 0), FigureType::King, side));

	/// and the same for the other side
	side = FigurePlayer::Blacks;

	for (int i = 0; i < 8; i++) {
		auto point = new PPoint(i, 6);
		auto pawns = new PFigure(point, FigureType::Pawn, side);
		m_board.push_back(pawns);
	}
	m_board.push_back(new PFigure(new PPoint(0, 7), FigureType::Rook, side));
	m_board.push_back(new PFigure(new PPoint(7, 7), FigureType::Rook, side));
	m_board.push_back(new PFigure(new PPoint(1, 7), FigureType::Knight, side));
	m_board.push_back(new PFigure(new PPoint(6, 7), FigureType::Knight, side));
	m_board.push_back(new PFigure(new PPoint(2, 7), FigureType::Bishop, side));
	m_board.push_back(new PFigure(new PPoint(5, 7), FigureType::Bishop, side));
	m_board.push_back(new PFigure(new PPoint(3, 7), FigureType::Queen, side));
	m_board.push_back(new PFigure(new PPoint(4, 7), FigureType::King, side));
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

std::list<PFigure *> PCheckboard::getAllFigures() const {
	std::list<PFigure *> out;
	for (const auto i: m_board)
		out.push_back(i);
	for (const auto i: m_deadFigures)
		out.push_back(i);
	return out;
}

std::list<PPoint *> PCheckboard::buildPath(PFigure *figure) {
	if (!figure) throw invalid_argument("Cannot build path for null pointer");

	auto type = figure->getType();
	list<PPoint *> possibleMoves;


	// build possible path for different figure types
	switch (type) {
		case Pawn:
			for (auto i: buildPawnPath(figure))
				possibleMoves.push_back(i);
			break;
		case Rook:
			for (auto i: buildRookPath(figure))
				possibleMoves.push_back(i);
			break;
		case Knight:
			for (auto i: buildKnightPath(figure))
				possibleMoves.push_back(i);
			break;
		case Bishop:
			for (auto i: buildBishopPath(figure))
				possibleMoves.push_back(i);
			break;
		case Queen:
			for (auto i: buildQueenPath(figure))
				possibleMoves.push_back(i);
			break;
		case King:
			for (auto i: buildKingPath(figure))
				possibleMoves.push_back(i);
			break;
	}
	return possibleMoves;
}


bool PCheckboard::allowsNext(PPoint *p, FigurePlayer side, bool isPawn) {
	auto fig = at(p);
	bool ret;
	if (isPawn)
		ret = fig && fig->getPlayer() != side;
	else
		ret = !fig || fig->getPlayer() != side;

	return ret;
}

bool PCheckboard::canGo(PPoint *p) {
	return p->getX() < 8 && p->getY() < 8;
}


std::list<PPoint *> PCheckboard::buildPawnPath(PFigure *figure) {
	BUILD_PATH_INTRO

	if (side == FigurePlayer::Whites) {
		addOrDie(new PPoint(x, y + 1));
		addOrDie(new PPoint(x + 1, y + 1), true);
		addOrDie(new PPoint(x - 1, y + 1), true);
		if (y == 1)
			addOrDie(new PPoint(x, y + 2));

	} else {
		addOrDie(new PPoint(x, y - 1));
		addOrDie(new PPoint(x + 1, y - 1), true);
		addOrDie(new PPoint(x - 1, y - 1), true);
		if (y == 6)
			addOrDie(new PPoint(x, y - 2));
	}
	return path;
}


std::list<PPoint *> PCheckboard::buildKnightPath(PFigure *figure) {
	BUILD_PATH_INTRO

	/// to the right
	addOrDie(new PPoint(x + 2, y + 1));
	addOrDie(new PPoint(x + 2, y - 1));

	/// to the left
	addOrDie(new PPoint(x - 2, y + 1));
	addOrDie(new PPoint(x - 2, y - 1));

	/// to the top
	addOrDie(new PPoint(x + 1, y + 2));
	addOrDie(new PPoint(x - 1, y + 2));

	/// to the bottom
	addOrDie(new PPoint(x + 1, y - 2));
	addOrDie(new PPoint(x - 1, y - 2));

	return path;
}

std::list<PPoint *> PCheckboard::buildRookPath(PFigure *figure) {
	BUILD_PATH_INTRO


	/// to the right
	for (int i = x + 1; i < 8; ++i) {
		auto p = new PPoint(i, y);
		if (!addOrDie(p)) break;
		auto f = at(p);
		if (f) break; // we cannot move through figures
	}

	/// to the left
	for (int i = x - 1; i >= 0; --i) {
		auto p = new PPoint(i, y);
		if (!addOrDie(p)) break;
		auto f = at(p);
		if (f) break; // we cannot move through figures
	}

	/// to the top
	for (int i = y + 1; i < 8; ++i) {
		auto p = new PPoint(x, i);
		if (!addOrDie(p)) break;
		auto f = at(p);
		if (f) break; // we cannot move through figures
	}

	/// to the bottom
	for (int i = y - 1; i >= 0; --i) {
		auto p = new PPoint(x, i);
		if (!addOrDie(p)) break;
		auto f = at(p);
		if (f) break; // we cannot move through figures
	}

	return path;
}

std::list<PPoint *> PCheckboard::buildBishopPath(PFigure *figure) {
	BUILD_PATH_INTRO

	bool good = true;
	/// to the right-top
	for (int i = x + 1, j = y + 1; j < 8 && i < 8 && good; ++i, ++j) {
		auto p = new PPoint(i, j);
		good = addOrDie(p);
		if (good) {
			auto f = at(p);
			if (f) good = false; // we cannot move through figures
		}
	}

	good = true;
	/// to the left-top
	for (int i = x - 1, j = y + 1; j < 8 && i >= 0 && good; --i, ++j) {
		auto p = new PPoint(i, j);
		good = addOrDie(p);
		if (good) {
			auto f = at(p);
			if (f) good = false; // we cannot move through figures
		}
	}

	good = true;
	/// to the right-bottom
	for (int i = x + 1, j = y - 1; j >= 0 && i < 8 && good; ++i, --j) {
		auto p = new PPoint(i, j);
		good = addOrDie(p);
		if (good) {
			auto f = at(p);
			if (f) good = false; // we cannot move through figures
		}
	}


	good = true;
	/// to the left-bottom
	for (int i = x - 1, j = y - 1; j >= 0 && i >= 0 && good; --i, --j) {
		auto p = new PPoint(i, j);
		good = addOrDie(p);
		if (good) {
			auto f = at(p);
			if (f) good = false; // we cannot move through figures
		}
	}

	return path;
}

std::list<PPoint *> PCheckboard::buildKingPath(PFigure *figure) {
	BUILD_PATH_INTRO


	addOrDie(new PPoint(x + 1, y));
	addOrDie(new PPoint(x - 1, y));

	addOrDie(new PPoint(x + 1, y + 1));
	addOrDie(new PPoint(x - 1, y + 1));
	addOrDie(new PPoint(x - 1, y - 1));
	addOrDie(new PPoint(x + 1, y - 1));

	addOrDie(new PPoint(x, y + 1));
	addOrDie(new PPoint(x, y - 1));

	return path;

}

std::list<PPoint *> PCheckboard::buildQueenPath(PFigure *figure) {
	BUILD_PATH_INTRO

	for (auto i: buildBishopPath(figure))
		path.push_back(i);
	for (auto i: buildRookPath(figure))
		path.push_back(i);

	return path;
}

void PCheckboard::setTurn(bool w) {
	whitesTurn = w;
}

bool PCheckboard::getWhitesTurn() const {
	return whitesTurn;
}
