//
// Created by Artem Martus on 06.04.2020.
//

#pragma once

#include <memory>

class PPoint;

enum FigureType : int { Pawn = 0, Rook, Knight, Bishop, Queen, King };

enum FigurePlayer : int { Whites = 0, Blacks };

/// PFigure class
class PFigure {
protected:
    std::shared_ptr<PPoint> position;
    std::shared_ptr<PFigure> killedBy;
    FigurePlayer player;
    FigureType type;
    unsigned int movesMade;

public:
    PFigure(const PPoint& point, FigureType type, FigurePlayer player,
            unsigned int movesMade = 0,
            std::shared_ptr<PFigure> killedBy = nullptr) noexcept;

    PFigure(const PFigure& figure) noexcept;

    ~PFigure();

    void isCapturedBy(const std::shared_ptr<PFigure>& by);

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

    std::shared_ptr<PPoint> getPoint() const;

    int getX() const;  // point alias

    int getY() const;  // point alias

    std::shared_ptr<PFigure> getKilledBy() const;

    void moved();

    unsigned int getMovesCount() const;

    bool operator==(const PFigure& figure) const;

    bool operator!=(const PFigure& figure) const;
};
