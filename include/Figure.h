//
// Created by Artem Martus on 06.04.2020.
//

#pragma once

#include <memory>

class Point;

enum FigureType : int { Pawn = 0, Rook, Knight, Bishop, Queen, King };

enum FigurePlayer : int { Whites = 0, Blacks };

/// Figure class
class Figure {
protected:
    std::shared_ptr<Point> position;
    std::shared_ptr<Figure> killedBy;
    FigurePlayer player;
    FigureType type;
    unsigned int movesMade;

public:
    Figure(const Point& point, FigureType type, FigurePlayer player,
            unsigned int movesMade = 0,
            std::shared_ptr<Figure> killedBy = nullptr) noexcept;

    Figure(const Figure& figure) noexcept;

    ~Figure();

    void isCapturedBy(const std::shared_ptr<Figure>& by);

    void revive();

    char asChar() const;

    bool isAlive() const;

    bool isReadyForCastling() const;

    bool isPawn() const;

    bool isRook() const;

    bool isKnight() const;

    bool isBishop() const;

    bool isQueen() const;

    bool isKing() const;

    FigureType getType() const;

    FigurePlayer getPlayer() const;

    std::shared_ptr<Point> getPoint() const;

    int getX() const;  // point alias

    int getY() const;  // point alias

    std::shared_ptr<Figure> getKilledBy() const;

    void moved();

    unsigned int getMovesCount() const;

    bool operator==(const Figure& figure) const;

    bool operator!=(const Figure& figure) const;
};
