//
// Created by Artem Martus on 10.04.2020.
//

#pragma once

#include <list>
#include <memory>

class Point;

class Figure;

enum FigurePlayer : int;

class FigureFactory {
public:
    static std::list<std::shared_ptr<Figure>> buildSide(FigurePlayer side);

    static std::list<std::shared_ptr<Figure>> buildPawns(FigurePlayer side);

    static std::list<std::shared_ptr<Figure>> buildRooks(FigurePlayer side);

    static std::list<std::shared_ptr<Figure>> buildKnights(FigurePlayer side);

    static std::list<std::shared_ptr<Figure>> buildBishops(FigurePlayer side);

    static std::shared_ptr<Figure> buildQueen(FigurePlayer side);

    static std::shared_ptr<Figure> buildKing(FigurePlayer side);
};

