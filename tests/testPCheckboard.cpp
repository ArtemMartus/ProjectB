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

		THEN ("If we try to reach existing figure we get it") {
			auto blackPawnSpot = make_shared<PPoint>(0, 6);
			auto blackPawn = at(blackPawnSpot);
			REQUIRE(blackPawn != nullptr);
		}

		THEN ("If we try to reach non-existing figure we don't get it") {
			auto uselessSpot = make_shared<PPoint>(4, 4);
			auto nothing = at(uselessSpot);
			REQUIRE(nothing == nullptr);
		}
	} WHEN("Pawn reaches end of a board") {
		auto allySide = Blacks, enemySide = Whites;
		m_board.push_back(PFigureFactory::buildKing(allySide));
		m_board.push_back(PFigureFactory::buildKing(enemySide));

		auto pawn = make_shared<PFigure>(PPoint(2, 1), Pawn, allySide);
		auto destinationPoint = make_shared<PPoint>(2, 0);
		m_board.push_back(pawn);
		auto deadQueen = make_shared<PFigure>(PPoint(1, 1), Queen, allySide);
		auto deadRook = make_shared<PFigure>(PPoint(1, 2), Rook, allySide);

		deadQueen->isCapturedBy(pawn);
		deadRook->isCapturedBy(pawn);

		THEN("we get queen if only queen is dead") {
			m_deadFigures.push_back(deadQueen);
			REQUIRE(prepareMove(pawn->getPoint(), destinationPoint));
			REQUIRE(!pawn->isAlive());
			REQUIRE(deadQueen->isAlive());
			REQUIRE(*deadQueen->getPoint() == *destinationPoint);
		} THEN("we get queen if queen and rook are dead") {
			m_deadFigures.push_back(deadQueen);
			m_deadFigures.push_back(deadRook);
			REQUIRE(pawn->isAlive());

			REQUIRE(prepareMove(pawn->getPoint(), destinationPoint));

			REQUIRE_FALSE(pawn->isAlive());
			REQUIRE(*pawn->getKilledBy() == *deadQueen);
			REQUIRE(deadQueen->isAlive());
			REQUIRE_FALSE(deadRook->isAlive());
			REQUIRE(*deadQueen->getPoint() == *destinationPoint);
		} THEN("we get a queen if only enemies are dead") {
			m_deadFigures.push_back(make_shared<PFigure>(PPoint(1, 1), Knight, enemySide));
			REQUIRE(prepareMove(pawn->getPoint(), destinationPoint));
			auto newCreature = m_board.back();
			REQUIRE_FALSE(pawn->isAlive());
			REQUIRE(m_board.size() == 3); // new queen + 2 kings
			REQUIRE(newCreature->isAlive());
			REQUIRE(*newCreature->getPoint() == *destinationPoint);
			REQUIRE(newCreature->isQueen());
		} THEN("we get a queen if none are dead") {
			REQUIRE(prepareMove(pawn->getPoint(), destinationPoint));
			auto newCreature = m_board.back();
			REQUIRE_FALSE(pawn->isAlive());
			REQUIRE(m_board.size() == 3);
			REQUIRE(newCreature->isAlive());
			REQUIRE(*newCreature->getPoint() == *destinationPoint);
			REQUIRE(newCreature->isQueen());
		}
	}

	WHEN ("A pawn captures enemy at the end of a map") {
		auto allySide = Blacks, enemySide = Whites;
		m_board.push_back(PFigureFactory::buildKing(allySide));
		m_board.push_back(PFigureFactory::buildKing(enemySide));

		auto pawn = make_shared<PFigure>(PPoint(2, 1), Pawn, allySide);
		auto destinationPoint = make_shared<PPoint>(1, 0);
		auto enemyRook = make_shared<PFigure>(*destinationPoint, Rook, enemySide);

		m_board.push_back(enemyRook);
		m_board.push_back(pawn);

		THEN("A pawn morphs into dead ally") {
			REQUIRE(prepareMove(pawn->getPoint(), destinationPoint));
			auto newCreature = m_board.back();
			REQUIRE_FALSE(pawn->isAlive());
			REQUIRE_FALSE(enemyRook->isAlive());
			REQUIRE(m_board.size() == 3);
			REQUIRE(newCreature->isAlive());
			REQUIRE(*newCreature->getPoint() == *destinationPoint);
			REQUIRE(newCreature->isQueen());
			REQUIRE(newCreature->getPlayer() == allySide);
		}
	}
}
