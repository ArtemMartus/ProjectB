//
// Created by Artem Martus on 06.04.2020.
//

#ifndef PROJECTB_PCHECKBOARD_H
#define PROJECTB_PCHECKBOARD_H

#include <list>
#include <memory>
#include <map>

class PPoint;

class PFigure;

class PPathSystem;

enum FigurePlayer : int;

class PCheckboard {
protected:

	std::list<std::shared_ptr<PFigure>> m_board;
	std::list<std::shared_ptr<PFigure>> m_deadFigures;
	std::shared_ptr<PPathSystem> m_pathSystem;
	bool whitesTurn = true;

	void destroy();

	virtual void performMovement(const std::shared_ptr<PFigure> &figure, const std::shared_ptr<PPoint> &toPlace);

public:
	PCheckboard();

	virtual ~PCheckboard();

	virtual std::shared_ptr<PFigure> at(const std::shared_ptr<PPoint> &point) const;

	/// returns true if move was successfully made
	virtual bool prepareMove(const std::shared_ptr<PPoint> &from, const std::shared_ptr<PPoint> &to);

	/// create fresh figures and place them on board
	virtual void initialize();

	virtual void setTurn(bool whitesTurn);

	virtual bool getWhitesTurn() const;

	virtual bool onePlayerLeft() const;

	virtual bool canMoveFrom(const std::shared_ptr<PFigure> &from) const;

	// save-load needed functions
	explicit PCheckboard(const std::list<std::shared_ptr<PFigure>> &figures) noexcept;

	virtual std::list<std::shared_ptr<PFigure>> getAllFigures() const;
};


#endif //PROJECTB_PCHECKBOARD_H
