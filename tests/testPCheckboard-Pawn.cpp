//
// Created by Artem Martus on 08.04.2020.
//

#include "PCheckboardTest.hpp"

TEST_CASE_METHOD(PCheckboard, "Testing pawn-related internal methods of checkboard", "[checkboard-pawn]") {
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
		blackPawns.clear();
		whitePawns.clear();
	}

		/// mechanics
	SECTION("Path testing", "[buildPath]") {
		WHEN("Has no moves yet") {
			auto figure = make_shared<PFigure>(PPoint(4, 1),
			                                   FigureType::Pawn, FigurePlayer::Whites);
			addFigure(figure);

			auto path = buildPath(figure);
			REQUIRE(path.size() == 2); // top, x2Top

			removeFigure(figure);
		}

		WHEN("Has some moves") {
			auto figure = make_shared<PFigure>(PPoint(4, 1),
			                                   FigureType::Pawn, FigurePlayer::Whites);
			addFigure(figure);
			figure->moved();

			auto path = buildPath(figure);
			REQUIRE(path.size() == 1); // top

			removeFigure(figure);
		}

		WHEN("Ally in front") {
			auto point = PPoint(4, 1);
			auto figure = make_shared<PFigure>(point,
			                                   FigureType::Pawn, FigurePlayer::Whites),
					ally = make_shared<PFigure>(PPoint(point.getX(), point.getY() + 1),
					                            figure->getType(), figure->getPlayer());

			addFigure(figure);
			addFigure(ally);

			auto path = buildPath(figure);

			REQUIRE(path.empty()); // no way

			removeFigure(figure);
			removeFigure(ally);
		}

		WHEN("Enemy in front") {
			auto point = PPoint(4, 1);
			auto figure = make_shared<PFigure>(point,
			                                   FigureType::Pawn, FigurePlayer::Whites);
			auto enemySide =
					figure->getPlayer() == FigurePlayer::Whites ? FigurePlayer::Blacks : figure->getPlayer();
			auto enemy = make_shared<PFigure>(
					PPoint(point.getX(), point.getY() + 1),
					figure->getType(), enemySide);

			addFigure(figure);
			addFigure(enemy);

			auto path = buildPath(figure);

			REQUIRE(path.empty()); // no way neither

			removeFigure(figure);
			removeFigure(enemy);
		}

		WHEN("Enemy in front-right") {
			auto point = PPoint(4, 1);
			auto figure = make_shared<PFigure>(point,
			                                   FigureType::Pawn, FigurePlayer::Whites);
			auto enemySide =
					figure->getPlayer() == FigurePlayer::Whites ? FigurePlayer::Blacks : figure->getPlayer();
			auto enemy = make_shared<PFigure>(
					PPoint(point.getX() + 1, point.getY() + 1),
					figure->getType(), enemySide);

			addFigure(figure);
			addFigure(enemy);

			auto path = buildPath(figure);

			REQUIRE(path.size() == 3); // top and attack!

			removeFigure(figure);
			removeFigure(enemy);
		}

		WHEN("Enemy in front-left") {
			auto point = PPoint(4, 1);
			auto figure = make_shared<PFigure>(point,
			                                   FigureType::Pawn, FigurePlayer::Whites, 1);
			auto enemySide =
					figure->getPlayer() == FigurePlayer::Whites ? FigurePlayer::Blacks : figure->getPlayer();
			auto enemy = make_shared<PFigure>(
					PPoint(point.getX() - 1, point.getY() + 1),
					figure->getType(), enemySide);

			addFigure(figure);
			addFigure(enemy);

			auto path = buildPath(figure);
			REQUIRE(path.size() == 2); // top and attack!

			removeFigure(figure);
			removeFigure(enemy);
		}
	}
}

