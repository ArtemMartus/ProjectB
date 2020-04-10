//
// Created by Artem Martus on 06.04.2020.
//

#include <stdexcept>
#include <cctype>
#include <utility>
#include "PFigure.h"
#include "PPoint.h"

using namespace std;

PFigure::PFigure(const PFigure &figure) noexcept
		: PFigure(*figure.getPoint(),
		          figure.getType(), figure.getPlayer(),
		          figure.getMovesCount(),
		          figure.getKilledBy()) {

}

PFigure::PFigure(const PPoint &a, FigureType b, FigurePlayer c,
                 unsigned int moves, shared_ptr<PFigure> k) noexcept
		: position(make_shared<PPoint>(a)), type(b), player(c), killedBy(std::move(k)), movesMade(moves) {
}

PFigure::~PFigure() {
	killedBy = nullptr;
	position = nullptr;
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

shared_ptr<PPoint> PFigure::getPoint() const {
	return position;
}

shared_ptr<PFigure> PFigure::getKilledBy() const {
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

void PFigure::capture(const shared_ptr<PFigure> &by) {
	if (!killedBy)
		killedBy = by;
	else
		throw std::invalid_argument("What is dead may never die");
}

bool PFigure::operator==(const PFigure &figure) const {
	return movesMade == figure.getMovesCount() &&
	       type == figure.getType() &&
	       player == figure.getPlayer();
}

bool PFigure::operator!=(const PFigure &figure) const {
	return !(*this == figure);
}