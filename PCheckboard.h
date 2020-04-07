//
// Created by Artem Martus on 06.04.2020.
//

#ifndef PROJECTB_PCHECKBOARD_H
#define PROJECTB_PCHECKBOARD_H

#include <list>
#include "PFigure.h"

class PPoint;

class PCheckboard {

	std::list<PFigure *> m_board;
	std::list<PFigure *> m_deadFigures;
	bool whitesTurn;

	void destroy();

	bool allowsNext(PPoint *point, PFigure::Player side, bool isPawn = false);

	bool canGo(PPoint *p);


public:
	PCheckboard();

	~PCheckboard();

	PFigure *at(PPoint *point);

	/// returns true if move was made
	bool move(PPoint *from, PPoint *to);

	std::list<PPoint *> buildPath(PFigure *figure);
	std::list<PPoint *> buildPawnPath(PFigure *figure);
	std::list<PPoint *> buildKnightPath(PFigure *figure);
	std::list<PPoint *> buildRookPath(PFigure *figure);
	std::list<PPoint *> buildBishopPath(PFigure *figure);
	std::list<PPoint *> buildQueenPath(PFigure* figure);
	std::list<PPoint *> buildKingPath(PFigure* figure);

	/// resets all to beginning
	void initialize();

	void setTurn(bool whitesTurn);
	bool getWhitesTurn() const ;

	std::list<PFigure *> getAliveFigures() const;

	bool onePlayerLeft() const;

	// save-load needed functions
	PCheckboard(std::list<PFigure *> figures);

	std::list<PFigure *> getAllFigures() const;
};


#endif //PROJECTB_PCHECKBOARD_H
