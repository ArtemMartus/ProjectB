//
// Created by Artem Martus on 06.04.2020.
//

#pragma once

#include <string>

/// This class represents point on a chessboard
class Point {
protected:
    unsigned int x, y;

public:
    Point(const Point& point) noexcept;

    Point(unsigned int x, unsigned int y) noexcept;

    unsigned int getX() const;

    unsigned int getY() const;

    bool inBounds() const; /// says whether coordinates in [0;7] range

    void setX(unsigned int);

    void setY(unsigned int);

    void shift(int dx, int dy);

    std::string asString() const;

    bool operator==(const Point& point) const;

    bool operator!=(const Point& point) const;
};

std::ostream& operator<<(std::ostream& o, const Point& p);
