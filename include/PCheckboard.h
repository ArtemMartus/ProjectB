//
// Created by Artem Martus on 06.04.2020.
//

#ifndef PROJECTB_PCHECKBOARD_H
#define PROJECTB_PCHECKBOARD_H

#include <list>
#include <memory>

class PPoint;

class PFigure;

enum FigurePlayer : int;

class PCheckboard {
protected:

#ifndef NDEBUG

	virtual void addFigure(const std::shared_ptr<PFigure> &figure); /// for testing ONLY

	virtual void removeFigure(const std::shared_ptr<PFigure> &figure); /// for testing ONLY
#endif

	std::list<std::shared_ptr<PFigure>> m_board;
	std::list<std::shared_ptr<PFigure>> m_deadFigures;
	bool whitesTurn = true;

	void destroy();

	virtual bool checkCastling(const PFigure &one, const PFigure &two);

	virtual std::shared_ptr<PPoint> addOrDie(const PPoint &point, FigurePlayer side, bool pawnMode);

	virtual std::list<std::shared_ptr<PPoint>> buildPawnPath(const PFigure &figure);

	virtual std::list<std::shared_ptr<PPoint>> buildKnightPath(const PFigure &figure);

	virtual std::list<std::shared_ptr<PPoint>> buildRookPath(const PFigure &figure);

	virtual std::list<std::shared_ptr<PPoint>> buildBishopPath(const PFigure &figure);

	virtual std::list<std::shared_ptr<PPoint>> buildKingPath(const PFigure &figure);

	virtual std::list<std::shared_ptr<PFigure>> buildSide(FigurePlayer side);

	virtual void performMovement(const std::shared_ptr<PFigure> &figure, const PPoint &toPlace);

	virtual std::list<std::shared_ptr<PFigure>> buildPawns(FigurePlayer side);

	virtual std::list<std::shared_ptr<PFigure>> buildRooks(FigurePlayer side);

	virtual std::list<std::shared_ptr<PFigure>> buildKnights(FigurePlayer side);

	virtual std::list<std::shared_ptr<PFigure>> buildBishops(FigurePlayer side);

	virtual std::shared_ptr<PFigure> buildQueen(FigurePlayer side);

	virtual std::shared_ptr<PFigure> buildKing(FigurePlayer side);

public:
	PCheckboard();

	virtual ~PCheckboard();

	virtual std::shared_ptr<PFigure> at(const PPoint &point);

	virtual std::list<std::shared_ptr<PPoint>> buildPath(const PFigure &figure);

	virtual std::list<std::shared_ptr<PFigure>> getListOfFiguresAvailableForMove(FigurePlayer side) const;

	/// returns true if move was made
	virtual bool prepareMove(const PPoint &from, const PPoint &to);

	/// create fresh figures and place them on board
	virtual void initialize();

	virtual void setTurn(bool whitesTurn);

	virtual bool getWhitesTurn() const;

	virtual bool onePlayerLeft() const;

	// save-load needed functions
	explicit PCheckboard(const std::list<std::shared_ptr<PFigure>> &figures) noexcept;

	virtual std::list<std::shared_ptr<PFigure>> getAllFigures() const;
};


#endif //PROJECTB_PCHECKBOARD_H
