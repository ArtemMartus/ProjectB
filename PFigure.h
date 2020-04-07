//
// Created by Artem Martus on 06.04.2020.
//

#ifndef PROJECTB_PFIGURE_H
#define PROJECTB_PFIGURE_H

class PPoint;

class PFigure {
	PPoint* position;
	PFigure* killedBy;
public: /// enums
	enum Type {
		Pawn, Rook, Knight, Bishop, Queen, King
	};
	enum Player {
		Whites, Blacks
	};

public:
	PFigure(PPoint* point, Type type, Player player);
	explicit PFigure(const PFigure* figure);
	~PFigure();

	void kill(PFigure* by);
	void revive();

	char asChar() const;
	bool isAlive() const;
	Type getType() const;
	Player getPlayer() const;
	PPoint* getPoint() const;
	PFigure* getKilledBy() const;

private:
	Player player;
	Type type;
};


#endif //PROJECTB_PFIGURE_H
