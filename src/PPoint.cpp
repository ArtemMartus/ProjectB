//
// Created by Artem Martus on 06.04.2020.
//

#include "PPoint.h"
#include <sstream>
#include <stdexcept>

using namespace std;

PPoint::PPoint(unsigned int a, unsigned int b) noexcept : x(a), y(b) {
}

PPoint::PPoint(const PPoint &point) noexcept : x(point.getX()), y(point.getY()) {

}

unsigned int PPoint::getX() const {
	return x;
}

unsigned int PPoint::getY() const {
	return y;
}

void PPoint::setX(unsigned int i) {
	x = i;
}

void PPoint::setY(unsigned int i) {
	y = i;
}

string PPoint::asString() const {
	ostringstream s;
	s << "(" << getX();
	s << ", " << getY();
	s << ")";
	return s.str();
}

bool PPoint::inBounds() const {
	return x < 8 && y < 8;
}

bool PPoint::operator==(const PPoint &b) const {
	return x == b.getX() && y == b.getY();
}

bool PPoint::operator!=(const PPoint &point) const {
	return !(*this == point);
}

ostream &operator<<(ostream &o, const PPoint &p) {
	o << p.asString();
	return o;
}