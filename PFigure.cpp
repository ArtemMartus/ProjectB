//
// Created by Artem Martus on 06.04.2020.
//

#include <stdexcept>
#include <cctype>
#include "PFigure.h"
#include "PPoint.h"

PFigure::PFigure(PPoint *a, Type b, Player c)
		: position(a), type(b), player(c), killedBy(nullptr) {
	if (a == nullptr) throw std::invalid_argument("Figure must have its place!");
}

PFigure::PFigure(const PFigure *figure)
		: position(new PPoint(figure->position)), type(figure->type),
		  player(figure->player), killedBy(nullptr) {
	if (figure->killedBy) {
		killedBy = new PFigure(figure->killedBy);
	}
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

void PFigure::kill(PFigure *by) {
	if (!by) throw std::invalid_argument("Cannot be killed by null pointer!");
	if (!killedBy)
		killedBy = new PFigure(by);
	else
		throw std::invalid_argument("What is dead may never die");
}

bool PFigure::isAlive() const {
	return killedBy == nullptr;
}

PFigure::Type PFigure::getType() const {
	return type;
}

PFigure::Player PFigure::getPlayer() const {
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

	return player == PFigure::Player::Whites ? toupper(out) : out;
}

void PFigure::revive() {
	delete killedBy;
	killedBy = nullptr;
}
