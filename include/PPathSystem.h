//
// Created by Artem Martus on 10.04.2020.
//

#pragma once

#include <list>
#include <memory>
#include <map>

class PFigure;

class PPoint;

enum FigurePlayer : int;

class PPathSystem {
    std::list<std::shared_ptr<PFigure>> board;

protected:
    std::shared_ptr<PFigure> at(const std::shared_ptr<PPoint> &point) const;

    bool checkCastling(const std::shared_ptr<PFigure> &one,
                       const std::shared_ptr<PFigure> &two) const;

    std::shared_ptr<PPoint> addOrDie(const std::shared_ptr<PPoint> &point,
                                     FigurePlayer side, bool pawnMode) const;

    std::list<std::shared_ptr<PPoint>> buildPawnPath(
            const std::shared_ptr<PFigure> &figure) const;

    std::list<std::shared_ptr<PPoint>> buildKnightPath(
            const std::shared_ptr<PFigure> &figure) const;

    std::list<std::shared_ptr<PPoint>> buildRookPath(
            const std::shared_ptr<PFigure> &figure) const;

    std::list<std::shared_ptr<PPoint>> buildBishopPath(
            const std::shared_ptr<PFigure> &figure) const;

    std::list<std::shared_ptr<PPoint>> buildKingPath(
            const std::shared_ptr<PFigure> &figure) const;

    std::multimap<std::shared_ptr<PFigure>, std::shared_ptr<PPoint>>
    getRawListOfMoves(FigurePlayer side) const;

    std::shared_ptr<PFigure> getKing(FigurePlayer side) const;

public:
    PPathSystem() noexcept;

    explicit PPathSystem(std::list<std::shared_ptr<PFigure>> board) noexcept;

    std::list<std::shared_ptr<PPoint>> buildPath(
            const std::shared_ptr<PFigure> &figure) const;

    const std::list<std::shared_ptr<PFigure>> &getBoard() const;

    void setBoard(const std::list<std::shared_ptr<PFigure>> &list);

    // returns true if move can be made
    bool checkForMovement(const std::shared_ptr<PFigure> &from,
                          const std::shared_ptr<PPoint> &to) const;

    std::list<std::shared_ptr<PPoint>> checkForAnyMovement(
            const std::shared_ptr<PFigure> &from) const;

    std::multimap<std::shared_ptr<PFigure>, std::shared_ptr<PPoint>>
    getListOfAvailableMoves(FigurePlayer side) const;
};
