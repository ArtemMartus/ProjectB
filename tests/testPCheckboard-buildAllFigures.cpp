//
// Created by Artem Martus on 09.04.2020.
//

#include "PCheckboardTest.hpp"

TEST_CASE_METHOD(PCheckboard, "Test if checkboard properly initialized", "[checkboard-initialize]") {
	WHEN("We run initialize method") {
		initialize();
		THEN("We got 16 pawns") {
			int blackPawn = 0;
			int whitePawn = 0;

			for (const auto& i :m_board) {
				if (!i->isPawn()) continue;
				if (i->getPlayer() == FigurePlayer::Whites)
					whitePawn++;
				else
					blackPawn++;
			}

			REQUIRE(blackPawn == 7);
			REQUIRE(whitePawn == 7);
		}
	}

	THEN("We got 4 rooks") {
		int blackRook = 0;
		int whiteRook = 0;

		for (const auto& i :m_board) {
			if (!i->isRook()) continue;
			if (i->getPlayer() == FigurePlayer::Whites)
				whiteRook++;
			else
				blackRook++;
		}

		REQUIRE(blackRook == 7);
		REQUIRE(whiteRook == 1);
	}
}
