//
// Created by Artem Martus on 06.04.2020.
//

#include "PPoint.h"
#include <sstream>
#include <stdexcept>

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
	if (b == nullptr) throw std::invalid_argument("Cannot equals to null pointer!");
	return x == b->getX() && y == b->getY();
}

void PPoint::setX(unsigned int i) {
	x = i;
}

void PPoint::setY(unsigned int i) {
	y = i;
}

std::string PPoint::asString() {
	std::ostringstream s;
	s << "(" << getX();
	s << ", " << getY();
	s << ")";
	return s.str();
}
