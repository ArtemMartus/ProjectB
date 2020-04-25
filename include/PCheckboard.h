//
// Created by Artem Martus on 06.04.2020.
//

#pragma once

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

    void performMovement(const std::shared_ptr<PFigure> &figure,
                         const std::shared_ptr<PPoint> &toPlace);

public:
    PCheckboard();

    ~PCheckboard();

    std::shared_ptr<PFigure> at(const std::shared_ptr<PPoint> &point) const;

    /// returns true if move was successfully made
    bool prepareMove(const std::shared_ptr<PPoint> &from,
                     const std::shared_ptr<PPoint> &to);

    /// create fresh figures and place them on board
    void initialize();

    void setTurn(bool whitesTurn);

    bool getWhitesTurn() const;

    bool onePlayerLeft() const;

    std::multimap<std::shared_ptr<PFigure>, std::shared_ptr<PPoint>> canMoveFrom(
            FigurePlayer side) const;

    // save-load needed functions
    explicit PCheckboard(
            const std::list<std::shared_ptr<PFigure>> &figures) noexcept;

    std::list<std::shared_ptr<PFigure>> getAllFigures() const;
};
