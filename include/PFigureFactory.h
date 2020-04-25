//
// Created by Artem Martus on 10.04.2020.
//

#pragma once

#include <list>
#include <memory>

class PPoint;

class PFigure;

enum FigurePlayer : int;

class PFigureFactory {
public:
    static std::list<std::shared_ptr<PFigure>> buildSide(FigurePlayer side);

    static std::list<std::shared_ptr<PFigure>> buildPawns(FigurePlayer side);

    static std::list<std::shared_ptr<PFigure>> buildRooks(FigurePlayer side);

    static std::list<std::shared_ptr<PFigure>> buildKnights(FigurePlayer side);

    static std::list<std::shared_ptr<PFigure>> buildBishops(FigurePlayer side);

    static std::shared_ptr<PFigure> buildQueen(FigurePlayer side);

    static std::shared_ptr<PFigure> buildKing(FigurePlayer side);
};

