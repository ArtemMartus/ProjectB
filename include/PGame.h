//
// Created by Artem Martus on 06.04.2020.
//

#ifndef PROJECTB_PGAME_H
#define PROJECTB_PGAME_H

#include <memory>

class PViewSide;

class PCheckboard;

class PSaver;

class PPoint;

class PFigure;

class PGame {
protected:
	PViewSide *view;
	PSaver *saver;
	PCheckboard *checkboard;

	std::shared_ptr<PFigure> selectFigure();

public:
	PGame(PViewSide *v, PSaver *saver);

	~PGame();

	bool run();

};


#endif //PROJECTB_PGAME_H
