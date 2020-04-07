//
// Created by Artem Martus on 06.04.2020.
//

#ifndef PROJECTB_PVIEWSIDE_H
#define PROJECTB_PVIEWSIDE_H

#include <list>
#include <string>
#include "PPoint.h"
#include "PFigure.h"

class PCheckboard;

class PViewSide {

	int inputAction(int lowBounds, int highBounds);

public:
	void renderText(std::string str);

	void renderText(char ch);

	void renderFigures(PCheckboard *checkboard);

	int askForAction(bool b, const std::list<std::string> &actions);

	PPoint *getPoint(const std::string& message);

	void renderKillText(char victim, char killer);

	void renderSelectedInfo(PFigure *pFigure);

	void renderMayGoToPath(const std::list<PPoint *>& list);
};


#endif //PROJECTB_PVIEWSIDE_H
