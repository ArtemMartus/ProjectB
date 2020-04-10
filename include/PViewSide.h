//
// Created by Artem Martus on 06.04.2020.
//

#ifndef PROJECTB_PVIEWSIDE_H
#define PROJECTB_PVIEWSIDE_H

#include <list>
#include <string>
#include <memory>
#include <set>

class PFigure;

class PPoint;

class PCheckboard;

class PViewSide {

	int inputAction(int lowBounds, int highBounds) const;

public:
	void renderText(const std::string &str) const;

	void renderFigures(const std::shared_ptr<PCheckboard> &checkboard) const;

	int askForAction(bool b, const std::list<std::string> &actions) const;

	std::shared_ptr<PPoint> getPoint(const std::string &message) const;

	void renderKillText(char victim, char killer) const;

	void renderSelectedInfo(const std::shared_ptr<PFigure> &pFigure) const;

	void renderMayGoToPath(const std::list<std::shared_ptr<PPoint>> &list) const;

	void renderFreeFigures(const std::set<std::shared_ptr<PFigure>> &set) const;
};


#endif //PROJECTB_PVIEWSIDE_H
