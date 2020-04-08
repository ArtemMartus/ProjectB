//
// Created by Artem Martus on 06.04.2020.
//

#include <stdexcept>
#include <cctype>
#include "PFigure.h"
#include "PPoint.h"

PFigure::PFigure(PPoint *a, FigureType b, FigurePlayer c, unsigned int moves)
		: position(a), type(b), player(c), killedBy(nullptr), movesMade(moves) {
	if (a == nullptr) throw std::invalid_argument("Figure must have its place!");
}

PFigure::~PFigure() {
	if (killedBy) {
		delete killedBy;
		killedBy = nullptr;
	}
	if (position) {
		delete position;
		position = nullptr;
	}
}

bool PFigure::isAlive() const {
	return killedBy == nullptr;
}

FigureType PFigure::getType() const {
	return type;
}

FigurePlayer PFigure::getPlayer() const {
	return player;
}

PPoint *PFigure::getPoint() const {
	return position;
}

PFigure *PFigure::getKilledBy() const {
	return killedBy;
}

char PFigure::asChar() const {
	char out = ' ';
	switch (type) {
		case Pawn:
			out = 'p';
			break;
		case Rook:
			out = 'r';
			break;
		case Knight:
			out = 'n';
			break;
		case Bishop:
			out = 'b';
			break;
		case Queen:
			out = 'q';
			break;
		case King:
			out = 'k';
			break;
	}

	return player == FigurePlayer::Whites ? toupper(out) : out;
}

void PFigure::revive() {
	delete killedBy;
	killedBy = nullptr;
}

unsigned int PFigure::getMovesCount() const {
	return movesMade;
}

void PFigure::moved() {
	++movesMade;
}

bool PFigure::readyForCastling() const {
	return isAlive() && movesMade == 0 && (type == FigureType::Rook || type == FigureType::King);
}

bool PFigure::isPawn() const {
	return type == FigureType::Pawn;
}

bool PFigure::isRook() const {
	return type == FigureType::Rook;
}

bool PFigure::isKnight() const {
	return type == FigureType::Knight;
}

bool PFigure::isBishop() const {
	return type == FigureType::Bishop;
}

bool PFigure::isQueen() const {
	return type == FigureType::Queen;
}

bool PFigure::isKing() const {
	return type == FigureType::King;
}

// further implementation

PFigureImpl::PFigureImpl(const PFigure *figure)
		: PFigure(new PPoint(figure->getPoint()),
		          figure->getType(), figure->getPlayer(),
		          figure->getMovesCount()) {

	if (figure->getKilledBy()) {
		killedBy = new PFigureImpl(figure->getKilledBy());
	}
}

void PFigureImpl::kill(PFigure *by) {
	if (!by) throw std::invalid_argument("Cannot be killed by null pointer!");
	if (!killedBy)
		killedBy = new PFigureImpl(by);
	else
		throw std::invalid_argument("What is dead may never die");
}

PFigureImpl::PFigureImpl(PPoint *point, FigureType type, FigurePlayer player, unsigned int movesMade)
		: PFigure(point,
		          type,
		          player,
		          movesMade) {
}
