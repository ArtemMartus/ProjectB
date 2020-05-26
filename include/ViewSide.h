//
// Created by Artem Martus on 06.04.2020.
//

#pragma once

#include <list>
#include <string>
#include <memory>
#include <set>

class Figure;

class Point;

class Checkboard;

class ViewSide {
    int inputAction(int lowBounds, int highBounds) const;

public:
    void renderText(const std::string &str) const;

    void renderFigures(const std::shared_ptr<Checkboard> &checkboard) const;

    int askForAction(bool b, const std::list<std::string> &actions) const;

    std::shared_ptr<Point> getPoint(const std::string &message) const;

    void renderKillText(char victim, char killer) const;

    void renderSelectedInfo(const std::shared_ptr<Figure> &Figure) const;

    void renderMayGoToPath(const std::list<std::shared_ptr<Point>> &list) const;

    void renderFreeFigures(const std::set<std::shared_ptr<Figure>> &set) const;
};
