//
// Created by Artem Martus on 06.04.2020.
//

#ifndef PROJECTB_PFIGURE_H
#define PROJECTB_PFIGURE_H

class PPoint;

enum FigureType : int {
	Pawn = 0, Rook, Knight, Bishop, Queen, King
};

enum FigurePlayer : int {
	Whites = 0, Blacks
};

/// PFigure abstract class
class PFigure {
protected:
	PPoint *position;
	PFigure *killedBy;
	FigurePlayer player;
	FigureType type;
	unsigned int movesMade;

public:
	PFigure(PPoint *point, FigureType type, FigurePlayer player, unsigned int movesMade = 0);

	virtual ~PFigure();

	virtual void kill(PFigure *by) = 0;

	virtual void revive();

	virtual char asChar() const;

	virtual bool isAlive() const;

	virtual bool readyForCastling() const;

	virtual bool isPawn() const;

	virtual bool isRook() const;

	virtual bool isKnight() const;

	virtual bool isBishop() const;

	virtual bool isQueen() const;

	virtual bool isKing() const;

	virtual FigureType getType() const;

	virtual FigurePlayer getPlayer() const;

	virtual PPoint *getPoint() const;

	virtual PFigure *getKilledBy() const;

	virtual void moved();

	virtual unsigned int getMovesCount() const;
};

/// Implementation figure
class PFigureImpl : public PFigure {

public:
	explicit PFigureImpl(const PFigure *figure);

	PFigureImpl(PPoint *point, FigureType type, FigurePlayer player, unsigned int movesMade = 0);

	void kill(PFigure *by) override;

};


#endif //PROJECTB_PFIGURE_H
