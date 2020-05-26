//
// Created by Artem Martus on 06.04.2020.
//

#pragma once

#include <list>
#include <map>
#include <memory>

class Point;

class Figure;

class PathSystem;

enum FigurePlayer : int;

class Checkboard {
protected:
    std::list<std::shared_ptr<Figure>> m_board;
    std::list<std::shared_ptr<Figure>> m_deadFigures;
    std::shared_ptr<PathSystem> m_pathSystem;
    bool whitesTurn = true;

    void destroy();

    void performMovement(const std::shared_ptr<Figure>& figure,
        const std::shared_ptr<Point>& toPlace);

public:
    Checkboard();

    ~Checkboard();

    std::shared_ptr<Figure> at(const std::shared_ptr<Point>& point) const;

    /// returns true if move was successfully made
    bool prepareMove(const std::shared_ptr<Point>& from,
        const std::shared_ptr<Point>& to);

    /// create fresh figures and place them on board
    void initialize();

    void setTurn(bool whitesTurn);

    bool getWhitesTurn() const;

    bool onePlayerLeft() const;

    std::multimap<std::shared_ptr<Figure>, std::shared_ptr<Point>> canMoveFrom(
        FigurePlayer side) const;

    // save-load needed functions
    explicit Checkboard(
        const std::list<std::shared_ptr<Figure>>& figures) noexcept;

    std::list<std::shared_ptr<Figure>> getAllFigures() const;
};
