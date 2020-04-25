//
// Created by Artem Martus on 08.04.2020.
//

#include "Tests.hpp"

TEST_CASE_METHOD(PPathSystem,
                 "Testing pawn-related internal methods of path system") {
    SECTION("Path testing", "[buildPath]") {
        WHEN("Has no moves yet") {
            auto figure = make_shared<PFigure>(PPoint(4, 1), FigureType::Pawn,
                                               FigurePlayer::Whites);

            setBoard({figure});
            auto path = buildPath(figure);
            REQUIRE(path.size() == 2);  // top, x2Top
        }

        WHEN("Has some moves") {
            auto figure = make_shared<PFigure>(PPoint(4, 1), FigureType::Pawn,
                                               FigurePlayer::Whites);
            setBoard({figure});
            figure->moved();

            auto path = buildPath(figure);
            REQUIRE(path.size() == 1);  // top
        }

        WHEN("Ally in front") {
            auto point = PPoint(4, 1);
            auto figure =
                    make_shared<PFigure>(point, FigureType::Pawn, FigurePlayer::Whites),
                    ally = make_shared<PFigure>(PPoint(point.getX(), point.getY() + 1),
                                                figure->getType(), figure->getPlayer());

            setBoard({figure, ally});

            auto path = buildPath(figure);

            REQUIRE(path.empty());  // no way
        }

        WHEN("Enemy in front") {
            auto point = PPoint(4, 1);
            auto figure =
                    make_shared<PFigure>(point, FigureType::Pawn, FigurePlayer::Whites);
            auto enemySide = figure->getPlayer() == FigurePlayer::Whites
                    ? FigurePlayer::Blacks
                    : figure->getPlayer();
            auto enemy = make_shared<PFigure>(PPoint(point.getX(), point.getY() + 1),
                                              figure->getType(), enemySide);

            setBoard({figure, enemy});

            auto path = buildPath(figure);

            REQUIRE(path.empty());  // no way neither
        }

        WHEN("Enemy in front-right") {
            auto point = PPoint(4, 1);
            auto figure =
                    make_shared<PFigure>(point, FigureType::Pawn, FigurePlayer::Whites);
            auto enemySide = figure->getPlayer() == FigurePlayer::Whites
                    ? FigurePlayer::Blacks
                    : figure->getPlayer();
            auto enemy =
                    make_shared<PFigure>(PPoint(point.getX() + 1, point.getY() + 1),
                                         figure->getType(), enemySide);

            setBoard({figure, enemy});

            auto path = buildPath(figure);

            REQUIRE(path.size() == 3);  // top and attack!
        }

        WHEN("Enemy in front-left") {
            auto point = PPoint(4, 1);
            auto figure = make_shared<PFigure>(point, FigureType::Pawn,
                                               FigurePlayer::Whites, 1);
            auto enemySide = figure->getPlayer() == FigurePlayer::Whites
                    ? FigurePlayer::Blacks
                    : figure->getPlayer();
            auto enemy =
                    make_shared<PFigure>(PPoint(point.getX() - 1, point.getY() + 1),
                                         figure->getType(), enemySide);

            setBoard({figure, enemy});

            auto path = buildPath(figure);
            REQUIRE(path.size() == 2);  // top and attack!
        }
    }
}
