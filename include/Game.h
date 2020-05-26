//
// Created by Artem Martus on 06.04.2020.
//

#pragma once

#include <memory>
#include <set>

class ViewSide;

class Checkboard;

class Saver;

class Point;

class Figure;

class Game {
    std::shared_ptr<ViewSide> view;
    std::shared_ptr<Saver> saver;
    std::shared_ptr<Checkboard> checkboard;

    std::shared_ptr<Figure> selectFigure(
        const std::set<std::shared_ptr<Figure>>& set);

public:
    Game(std::shared_ptr<ViewSide> viewSide,
        std::shared_ptr<Saver> saver) noexcept;

    ~Game();

    bool run();
};
