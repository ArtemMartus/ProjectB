//
// Created by Artem Martus on 08.04.2020.
//

#include "catch.hpp"

#include <PCheckboard.h>
#include <PFigure.h>
#include <PPoint.h>

TEST_CASE_METHOD(PCheckboard, "Testing internal methods of checkboard", "[checkboard]") {

	SECTION("Pawn") {
		WHEN("Build pawns create 8 entries") {
			auto whitePawns = buildPawns(FigurePlayer::Whites);
			auto blackPawns = buildPawns(FigurePlayer::Blacks);

			REQUIRE(whitePawns.size() == 8);
			REQUIRE(blackPawns.size() == 8);

			THEN("Correct white pawn placement") {
				int x = 0;
				for (auto i: whitePawns) {
					REQUIRE(i->getPoint()->getX() == x);
					REQUIRE(i->getPoint()->getY() == 1);
					REQUIRE(i->getMovesCount() == 0);
					REQUIRE(i->getPlayer() == FigurePlayer::Whites);
					REQUIRE(i->getType() == FigureType::Pawn);
					REQUIRE(i->getKilledBy() == nullptr);
					x++;
				}
			} AND_THEN("Correct black pawn placement") {
				int x = 0;
				for (auto i: blackPawns) {
					REQUIRE(i->getPoint()->getX() == x);
					REQUIRE(i->getPoint()->getY() == 6);
					REQUIRE(i->getMovesCount() == 0);
					REQUIRE(i->getPlayer() == FigurePlayer::Blacks);
					REQUIRE(i->getType() == FigureType::Pawn);
					REQUIRE(i->getKilledBy() == nullptr);
					x++;
				}
			}

			/// clean-up memory
			whitePawns.splice(whitePawns.begin(), blackPawns);
			for (auto i: whitePawns)
				delete i;
			whitePawns.clear();
		}

			/// mechanics
		SECTION("Path testing", "[buildPath]") {
			WHEN("Has no moves yet") {
				PFigure *figure = new PFigureImpl(new PPoint(4, 1),
				                                  FigureType::Pawn, FigurePlayer::Whites);
				addFigure(figure);

				auto path = buildPath(figure);
				REQUIRE(path.size() == 2); // top, x2Top

				removeFigure(figure);
				delete figure;
			}

			WHEN("Has some moves") {
				PFigure *figure = new PFigureImpl(new PPoint(4, 1),
				                                  FigureType::Pawn, FigurePlayer::Whites);
				addFigure(figure);
				figure->moved();

				auto path = buildPath(figure);
				REQUIRE(path.size() == 1); // top

				removeFigure(figure);
				delete figure;
			}

			WHEN("Ally in front") {
				PFigure *figure = new PFigureImpl(new PPoint(4, 1),
				                                  FigureType::Pawn, FigurePlayer::Whites),
						*ally = new PFigureImpl(
						new PPoint(figure->getPoint()->getX(), figure->getPoint()->getY() + 1),
						figure->getType(), figure->getPlayer());

				addFigure(figure);
				addFigure(ally);

				auto path = buildPath(figure);

				REQUIRE(path.empty()); // no way

				removeFigure(figure);
				removeFigure(ally);
				delete ally;
				delete figure;
			}

			WHEN("Enemy in front") {
				PFigure *figure = new PFigureImpl(new PPoint(4, 1),
				                                  FigureType::Pawn, FigurePlayer::Whites);
				auto enemySide =
						figure->getPlayer() == FigurePlayer::Whites ? FigurePlayer::Blacks : figure->getPlayer();
				PFigure *enemy = new PFigureImpl(
						new PPoint(figure->getPoint()->getX(), figure->getPoint()->getY() + 1),
						figure->getType(), enemySide);

				addFigure(figure);
				addFigure(enemy);

				auto path = buildPath(figure);
				for (auto i :path)
					WARN(i->asString());

				REQUIRE(path.size() == 0); // no way neither

				removeFigure(figure);
				removeFigure(enemy);
				delete enemy;
				delete figure;
			}

			WHEN("Enemy in front-right") {
				PFigure *figure = new PFigureImpl(new PPoint(4, 1),
				                                  FigureType::Pawn, FigurePlayer::Whites);
				auto enemySide =
						figure->getPlayer() == FigurePlayer::Whites ? FigurePlayer::Blacks : figure->getPlayer();
				PFigure *enemy = new PFigureImpl(
						new PPoint(figure->getPoint()->getX() + 1, figure->getPoint()->getY() + 1),
						figure->getType(), enemySide);

				addFigure(figure);
				addFigure(enemy);

				auto path = buildPath(figure);

				REQUIRE(path.size() == 3); // top and attack!

				removeFigure(figure);
				removeFigure(enemy);
				delete enemy;
				delete figure;
			}

			WHEN("Enemy in front-left") {
				PFigure *figure = new PFigureImpl(new PPoint(4, 1),
				                                  FigureType::Pawn, FigurePlayer::Whites, 1);
				auto enemySide =
						figure->getPlayer() == FigurePlayer::Whites ? FigurePlayer::Blacks : figure->getPlayer();
				PFigure *enemy = new PFigureImpl(
						new PPoint(figure->getPoint()->getX() - 1, figure->getPoint()->getY() + 1),
						figure->getType(), enemySide);

				addFigure(figure);
				addFigure(enemy);

				auto path = buildPath(figure);

				REQUIRE(path.size() == 2); // top and attack!

				removeFigure(figure);
				removeFigure(enemy);
				delete enemy;
				delete figure;
			}
		}
	}
}
