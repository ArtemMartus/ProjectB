//
// Created by Artem Martus on 09.04.2020.
//

#include "Tests.hpp"

TEST_CASE_METHOD(PCheckboard, "Test if checkboard properly initialized", "[checkboard-initialize]") {
	WHEN("We run initialize method") {
		initialize();
		THEN("We got 16 pawns") {
			int blackPawn = 0;
			int whitePawn = 0;

			for (const auto &i :m_board) {
				if (!i->isPawn()) continue;
				if (i->getPlayer() == FigurePlayer::Whites)
					whitePawn++;
				else
					blackPawn++;
			}

			REQUIRE(blackPawn == 8);
			REQUIRE(whitePawn == 8);
		}

		THEN("We got 4 rooks") {
			int blackRook = 0;
			int whiteRook = 0;

			for (const auto &i :m_board) {
				if (!i->isRook()) continue;
				if (i->getPlayer() == FigurePlayer::Whites)
					whiteRook++;
				else
					blackRook++;
			}

			REQUIRE(blackRook == 2);
			REQUIRE(whiteRook == 2);
		}

		THEN("We got 4 knights") {
			int blackKnight = 0;
			int whiteKnight = 0;

			for (const auto &i :m_board) {
				if (!i->isKnight()) continue;
				if (i->getPlayer() == FigurePlayer::Whites)
					whiteKnight++;
				else
					blackKnight++;
			}

			REQUIRE(blackKnight == 2);
			REQUIRE(whiteKnight == 2);
		}

		THEN("We got 4 bishops") {
			int blackBishops = 0;
			int whiteBishops = 0;

			for (const auto &i :m_board) {
				if (!i->isBishop()) continue;
				if (i->getPlayer() == FigurePlayer::Whites)
					whiteBishops++;
				else
					blackBishops++;
			}

			REQUIRE(blackBishops == 2);
			REQUIRE(whiteBishops == 2);
		}

		THEN("We got 2 queens") {
			bool blackQueen = false;
			bool whiteQueen = false;

			for (const auto &i :m_board) {
				if (!i->isQueen()) continue;
				if (i->getPlayer() == FigurePlayer::Whites)
					whiteQueen = true;
				else
					blackQueen = true;
			}

			REQUIRE(whiteQueen);
			REQUIRE(blackQueen);
		}

		THEN("We got 2 kings") {
			bool blackKing = false;
			bool whiteKing = false;

			for (const auto &i :m_board) {
				if (!i->isQueen()) continue;
				if (i->getPlayer() == FigurePlayer::Whites)
					whiteKing = true;
				else
					blackKing = true;
			}

			REQUIRE(whiteKing);
			REQUIRE(blackKing);
		}

	}
}
