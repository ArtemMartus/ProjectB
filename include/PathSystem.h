//
// Created by Artem Martus on 10.04.2020.
//

#pragma once

#include <list>
#include <memory>
#include <map>

class Figure;

class Point;

enum FigurePlayer : int;

class PathSystem {
    std::list<std::shared_ptr<Figure>> board;

protected:
    std::shared_ptr<Figure> at(const std::shared_ptr<Point> &point) const;

    bool checkCastling(const std::shared_ptr<Figure> &one,
                       const std::shared_ptr<Figure> &two) const;

    std::shared_ptr<Point> addOrDie(const std::shared_ptr<Point> &point,
                                     FigurePlayer side, bool pawnMode) const;

    std::list<std::shared_ptr<Point>> buildPawnPath(
            const std::shared_ptr<Figure> &figure) const;

    std::list<std::shared_ptr<Point>> buildKnightPath(
            const std::shared_ptr<Figure> &figure) const;

    std::list<std::shared_ptr<Point>> buildRookPath(
            const std::shared_ptr<Figure> &figure) const;

    std::list<std::shared_ptr<Point>> buildBishopPath(
            const std::shared_ptr<Figure> &figure) const;

    std::list<std::shared_ptr<Point>> buildKingPath(
            const std::shared_ptr<Figure> &figure) const;

    std::multimap<std::shared_ptr<Figure>, std::shared_ptr<Point>>
    getRawListOfMoves(FigurePlayer side) const;

    std::shared_ptr<Figure> getKing(FigurePlayer side) const;

public:
    PathSystem() noexcept;

    explicit PathSystem(std::list<std::shared_ptr<Figure>> board) noexcept;

    std::list<std::shared_ptr<Point>> buildPath(
            const std::shared_ptr<Figure> &figure) const;

    const std::list<std::shared_ptr<Figure>> &getBoard() const;

    void setBoard(const std::list<std::shared_ptr<Figure>> &list);

    // returns true if move can be made
    bool checkForMovement(const std::shared_ptr<Figure> &from,
                          const std::shared_ptr<Point> &to) const;

    std::list<std::shared_ptr<Point>> checkForAnyMovement(
            const std::shared_ptr<Figure> &from) const;

    std::multimap<std::shared_ptr<Figure>, std::shared_ptr<Point>>
    getListOfAvailableMoves(FigurePlayer side) const;
};
