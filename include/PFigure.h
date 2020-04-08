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

class PFigure {
	PPoint *position;
	PFigure *killedBy;
	FigurePlayer player;
	FigureType type;

public:
	PFigure(PPoint *point, FigureType type, FigurePlayer player);

	explicit PFigure(const PFigure *figure);

	~PFigure();

	void kill(PFigure *by);

	void revive();

	char asChar() const;

	bool isAlive() const;

	FigureType getType() const;

	FigurePlayer getPlayer() const;

	PPoint *getPoint() const;

	PFigure *getKilledBy() const;

};


#endif //PROJECTB_PFIGURE_H
