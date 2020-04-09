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

	virtual bool checkCastling(const std::shared_ptr<PFigure> &one, const std::shared_ptr<PFigure> &two) const;

	virtual std::shared_ptr<PPoint> addOrDie(const std::shared_ptr<PPoint> &point, FigurePlayer side, bool pawnMode) const;

	virtual std::list<std::shared_ptr<PPoint>> buildPawnPath(const std::shared_ptr<PFigure> &figure) const;

	virtual std::list<std::shared_ptr<PPoint>> buildKnightPath(const std::shared_ptr<PFigure> &figure) const;

	virtual std::list<std::shared_ptr<PPoint>> buildRookPath(const std::shared_ptr<PFigure> &figure) const;

	virtual std::list<std::shared_ptr<PPoint>> buildBishopPath(const std::shared_ptr<PFigure> &figure) const;

	virtual std::list<std::shared_ptr<PPoint>> buildKingPath(const std::shared_ptr<PFigure> &figure) const;

	virtual std::list<std::shared_ptr<PFigure>> buildSide(FigurePlayer side) const;

	virtual void performMovement(const std::shared_ptr<PFigure> &figure, const std::shared_ptr<PPoint> &toPlace);

	virtual std::list<std::shared_ptr<PFigure>> buildPawns(FigurePlayer side) const;

	virtual std::list<std::shared_ptr<PFigure>> buildRooks(FigurePlayer side) const;

	virtual std::list<std::shared_ptr<PFigure>> buildKnights(FigurePlayer side) const;

	virtual std::list<std::shared_ptr<PFigure>> buildBishops(FigurePlayer side) const;

	virtual std::shared_ptr<PFigure> buildQueen(FigurePlayer side) const;

	virtual std::shared_ptr<PFigure> buildKing(FigurePlayer side) const;

public:
	PCheckboard();

	virtual ~PCheckboard();

	virtual std::shared_ptr<PFigure> at(const std::shared_ptr<PPoint> &point) const;

	virtual std::list<std::shared_ptr<PPoint>> buildPath(const std::shared_ptr<PFigure> &figure) const;

	virtual std::list<std::shared_ptr<PFigure>> getListOfFiguresAvailableForMove(FigurePlayer side) const;

	/// returns true if move was made
	virtual bool prepareMove(const std::shared_ptr<PPoint> &from, const std::shared_ptr<PPoint> &to);

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
