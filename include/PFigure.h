//
// Created by Artem Martus on 06.04.2020.
//

#ifndef PROJECTB_PFIGURE_H
#define PROJECTB_PFIGURE_H

#include <memory>

class PPoint;

enum FigureType : int {
	Pawn = 0, Rook, Knight, Bishop, Queen, King
};

enum FigurePlayer : int {
	Whites = 0, Blacks
};

/// PFigure class
class PFigure {
protected:
	std::shared_ptr<PPoint> position;
	std::shared_ptr<PFigure> killedBy;
	FigurePlayer player;
	FigureType type;
	unsigned int movesMade;

public:
	PFigure(const PPoint &point, FigureType type, FigurePlayer player,
	        unsigned int movesMade = 0, std::shared_ptr<PFigure> killedBy = nullptr) noexcept;

	PFigure(const PFigure &figure) noexcept;

	virtual ~PFigure();

	virtual void isCapturedBy(const std::shared_ptr<PFigure> &by);

	virtual void revive();

	virtual char asChar() const;

	virtual bool isAlive() const;

	virtual bool isReadyForCastling() const;

	virtual bool isPawn() const;

	virtual bool isRook() const;

	virtual bool isKnight() const;

	virtual bool isBishop() const;

	virtual bool isQueen() const;

	virtual bool isKing() const;

	virtual FigureType getType() const;

	virtual FigurePlayer getPlayer() const;

	virtual std::shared_ptr<PPoint> getPoint() const;

	virtual int getX() const; //point alias

	virtual int getY() const; //point alias

	virtual std::shared_ptr<PFigure> getKilledBy() const;

	virtual void moved();

	virtual unsigned int getMovesCount() const;

	virtual bool operator==(const PFigure &figure) const;

	virtual bool operator!=(const PFigure &figure) const;
};


#endif //PROJECTB_PFIGURE_H
