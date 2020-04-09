//
// Created by Artem Martus on 09.04.2020.
//


#include "catch.hpp"
#include <memory>

#include <PCheckboard.h>
#include <PFigure.h>
#include <PPoint.h>

using namespace std;

TEST_CASE_METHOD(PCheckboard, "Test the king movement system", "[checkboard-king]") {
	WHEN("There is one king in the middle of a map") {
		auto king = make_shared<PFigure>(PPoint(3,3), FigureType::King, FigurePlayer::Whites);
		addFigure(king);
		auto path = buildPath(*king);

		THEN ("Can move everywhere") {
			REQUIRE(path.size() == 8); // top topRight right rightBot bot botLeft left leftTop
		}
		removeFigure(king);
	}
	WHEN ("King moved to the top right corner") {
		auto king = make_shared<PFigure>(PPoint(7,7), FigureType::King, FigurePlayer::Whites);
		addFigure(king);
		auto path = buildPath(*king);

		THEN("Can move only in three directions left-down'wards") {
			REQUIRE(path.size() == 3); // bot botLeft left
		}
		removeFigure(king);
	}
	WHEN ("King stands on a line with enemy rook") {
		auto king = make_shared<PFigure>(PPoint(7,7), FigureType::King, FigurePlayer::Whites);
		auto enemyRook = make_shared<PFigure>(PPoint(0,7),FigureType::Rook,FigurePlayer::Blacks);
		addFigure(king);
		addFigure(enemyRook);
		auto path = buildPath(*king);

		THEN("Can move only in two directions down'wards") {
			REQUIRE(path.size() == 2); // bot botLeft
		}
		THEN ("King cannot be killed") {
			REQUIRE_FALSE(prepareMove(*(enemyRook->getPoint()),*(king->getPoint())));
		}
		removeFigure(king);
		removeFigure(enemyRook);
	}
	WHEN ("King stands on a line with enemy rook and ally bishop can kill enemy rook") {
		auto king = make_shared<PFigure>(PPoint(7,7), FigureType::King, FigurePlayer::Whites);
		auto enemyRook = make_shared<PFigure>(PPoint(0,7),FigureType::Rook,FigurePlayer::Blacks);
		auto allyBishop = make_shared<PFigure>(PPoint(7,0),FigureType::Bishop,FigurePlayer::Whites);
		addFigure(king);
		addFigure(enemyRook);
		addFigure(allyBishop);
		auto path = buildPath(*king);
		auto bishopPath = buildPath(*allyBishop);

		auto myList = getListOfFiguresAvailableForMove(FigurePlayer::Whites);

		THEN("Can move with a king or use bishop to kill enemy") {
			REQUIRE(path.size() == 2); // bot botLeft
			REQUIRE(myList.size() == 2); // king and the allyBishop
			REQUIRE(bishopPath.size() == 1); // only one turn to get that rook
		}
		removeFigure(king);
		removeFigure(enemyRook);
		removeFigure(allyBishop);
	}
}
