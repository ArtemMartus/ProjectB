//
// Created by Artem Martus on 06.04.2020.
//

#include "PPoint.h"
#include <sstream>
#include <stdexcept>

using namespace std;

PPoint::PPoint(unsigned int a, unsigned int b) : x(a), y(b) {
}

PPoint::PPoint(const PPoint *point) : x(point->x), y(point->y) {

}

unsigned int PPoint::getX() const {
	return x;
}

unsigned int PPoint::getY() const {
	return y;
}

bool PPoint::isEquals(PPoint *b) {
	if (b == nullptr) throw invalid_argument("Cannot equals to null pointer!");
	return x == b->getX() && y == b->getY();
}

void PPoint::setX(unsigned int i) {
	x = i;
}

void PPoint::setY(unsigned int i) {
	y = i;
}

string PPoint::asString() {
	ostringstream s;
	s << "(" << getX();
	s << ", " << getY();
	s << ")";
	return s.str();
}

bool PPoint::inBounds() const {
	return x < 8 && y < 8;
}
