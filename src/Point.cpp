//
// Created by Artem Martus on 06.04.2020.
//

#include <Point.h>
#include <sstream>

#ifdef _WIN32

#include <stdexcept>

#endif

using namespace std;

Point::Point(unsigned int a, unsigned int b) noexcept
    : x(a)
    , y(b)
{
}

Point::Point(const Point& point) noexcept
    : x(point.getX())
    , y(point.getY())
{
}

unsigned int Point::getX() const { return x; }

unsigned int Point::getY() const { return y; }

void Point::setX(unsigned int i) { x = i; }

void Point::setY(unsigned int i) { y = i; }

string Point::asString() const
{
    ostringstream s;
    s << "(" << getX();
    s << ", " << getY();
    s << ")";
    return s.str();
}

bool Point::inBounds() const { return x < 8 && y < 8; }

bool Point::operator==(const Point& b) const
{
    return x == b.getX() && y == b.getY();
}

bool Point::operator!=(const Point& point) const { return !(*this == point); }

void Point::shift(int dx, int dy)
{
    x += dx;
    y += dy;
}

ostream& operator<<(ostream& o, const Point& p)
{
    o << p.asString();
    return o;
}
