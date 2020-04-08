//
// Created by Artem Martus on 06.04.2020.
//

#ifndef PROJECTB_PCHECKBOARD_H
#define PROJECTB_PCHECKBOARD_H

#include <list>

class PPoint;

class PFigure;

enum FigurePlayer : int;

class PCheckboard {
protected:

	std::list<PFigure *> m_board;
	std::list<PFigure *> m_deadFigures;
	bool whitesTurn = true;

	virtual void destroy();

	virtual bool checkCastling(PFigure *one, PFigure *two);

	virtual PPoint *addOrDie(PPoint *point, FigurePlayer side, bool pawnMode);

	virtual std::list<PPoint *> buildPawnPath(PFigure *figure);

	virtual std::list<PPoint *> buildKnightPath(PFigure *figure);

	virtual std::list<PPoint *> buildRookPath(PFigure *figure);

	virtual std::list<PPoint *> buildBishopPath(PFigure *figure);

	virtual std::list<PPoint *> buildKingPath(PFigure *figure);

	virtual std::list<PFigure *> buildSide(FigurePlayer side);

	virtual void performMovement(PFigure *figure, PPoint *toPlace);

	virtual std::list<PFigure *> buildPawns(FigurePlayer side);

	virtual std::list<PFigure *> buildRooks(FigurePlayer side);

	virtual std::list<PFigure *> buildKnights(FigurePlayer side);

	virtual std::list<PFigure *> buildBishops(FigurePlayer side);

	virtual PFigure *buildQueen(FigurePlayer side);

	virtual PFigure *buildKing(FigurePlayer side);

	virtual void addFigure(PFigure* figure); /// for testing

	virtual void removeFigure(PFigure* figure); /// for testing

public:
	PCheckboard();

	virtual ~PCheckboard();

	virtual PFigure *at(PPoint *point);

	virtual std::list<PPoint *> buildPath(PFigure *figure);

	/// returns true if move was made
	virtual bool prepareMove(PPoint *from, PPoint *to);

	/// create fresh figures and place them on board
	virtual void initialize();

	virtual void setTurn(bool whitesTurn);

	virtual bool getWhitesTurn() const;

	virtual std::list<PFigure *> getAliveFigures() const;

	virtual bool onePlayerLeft() const;

	// save-load needed functions
	PCheckboard(std::list<PFigure *> figures);

	virtual std::list<PFigure *> getAllFigures() const;
};


#endif //PROJECTB_PCHECKBOARD_H
