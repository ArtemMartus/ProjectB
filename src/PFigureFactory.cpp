//
// Created by Artem Martus on 10.04.2020.
//

#include <PFigureFactory.h>
#include <PFigure.h>
#include <PPoint.h>

using namespace std;

list<shared_ptr<PFigure>> PFigureFactory::buildSide(FigurePlayer side) {
    list<shared_ptr<PFigure>> figures;  // chessboard consists of 8x8 squares

    figures.splice(figures.end(), buildPawns(side));    /// 8 pawns
    figures.splice(figures.end(), buildRooks(side));    /// 2 rooks
    figures.splice(figures.end(), buildKnights(side));  /// 2 knights
    figures.splice(figures.end(), buildBishops(side));  /// 2 bishops
    figures.push_back(buildQueen(side));                /// one queen
    figures.push_back(buildKing(side));                 /// one king

    return figures;
}

list<shared_ptr<PFigure>> PFigureFactory::buildPawns(FigurePlayer side) {
    list<shared_ptr<PFigure>> pawns;
    auto pawnY = side == FigurePlayer::Whites ? 1 : 6;
    for (int i = 0; i < 8; i++)
        pawns.push_back(
                    make_shared<PFigure>(PPoint(i, pawnY), FigureType::Pawn, side));

    return pawns;
}

#define BUILD_FIGURES_Y \
    ;                     \
    auto y = side == FigurePlayer::Whites ? 7 : 0;  // 7 for whites, 0 for blacks

list<shared_ptr<PFigure>> PFigureFactory::buildRooks(FigurePlayer side) {
    BUILD_FIGURES_Y;

    return {make_shared<PFigure>(PPoint(0, 7 - y), FigureType::Rook, side),
                make_shared<PFigure>(PPoint(7, 7 - y), FigureType::Rook, side)};
}

list<shared_ptr<PFigure>> PFigureFactory::buildKnights(FigurePlayer side) {
    BUILD_FIGURES_Y;

    return {make_shared<PFigure>(PPoint(1, 7 - y), FigureType::Knight, side),
                make_shared<PFigure>(PPoint(6, 7 - y), FigureType::Knight, side)};
}

list<shared_ptr<PFigure>> PFigureFactory::buildBishops(FigurePlayer side) {
    BUILD_FIGURES_Y;

    return {make_shared<PFigure>(PPoint(2, 7 - y), FigureType::Bishop, side),
                make_shared<PFigure>(PPoint(5, 7 - y), FigureType::Bishop, side)};
}

shared_ptr<PFigure> PFigureFactory::buildQueen(FigurePlayer side) {
    BUILD_FIGURES_Y;

    return make_shared<PFigure>(PPoint(3, 7 - y), FigureType::Queen, side);
}

shared_ptr<PFigure> PFigureFactory::buildKing(FigurePlayer side) {
    BUILD_FIGURES_Y;

    return make_shared<PFigure>(PPoint(4, 7 - y), FigureType::King, side);
}
