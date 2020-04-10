//
// Created by Artem Martus on 10.04.2020.
//

#include "Tests.hpp"

TEST_CASE("Testing  figure factory") {
	WHEN("Build pawns create 8 entries") {
		auto whitePawns = PFigureFactory::buildPawns(FigurePlayer::Whites);
		auto blackPawns = PFigureFactory::buildPawns(FigurePlayer::Blacks);

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

}