//
// Created by Artem Martus on 06.04.2020.
//

#ifndef PROJECTB_PPOINT_H
#define PROJECTB_PPOINT_H

#include <string>

class PPoint {
	unsigned int x, y;
public:
	explicit PPoint(const PPoint* point);
	PPoint(unsigned int x, unsigned int y);
	bool isEquals(PPoint* b);
	unsigned int getX() const;
	unsigned int getY() const;
	void setX(unsigned int);
	void setY(unsigned int);
	std::string asString();
};


#endif //PROJECTB_PPOINT_H
