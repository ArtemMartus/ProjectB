//
// Created by Artem Martus on 06.04.2020.
//

#ifndef PROJECTB_PPOINT_H
#define PROJECTB_PPOINT_H

#include <string>

/// This class represents point on a chessboard
class PPoint {
protected:
	unsigned int x, y;
public:
	PPoint(const PPoint &point) noexcept;

	PPoint(unsigned int x, unsigned int y) noexcept;

	virtual unsigned int getX() const;

	virtual unsigned int getY() const;

	virtual bool inBounds() const; /// says whether coordinates in [0;7] range

	virtual void setX(unsigned int);

	virtual void setY(unsigned int);

	virtual void shift(int dx, int dy);

	virtual std::string asString() const;

	virtual bool operator==(const PPoint &point) const;

	virtual bool operator!=(const PPoint &point) const;
};

std::ostream &operator<<(std::ostream &o, const PPoint &p);

#endif //PROJECTB_PPOINT_H
