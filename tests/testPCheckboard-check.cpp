//
// Created by Artem Martus on 09.04.2020.
//


#include "catch.hpp"
#include <memory>

#include <PCheckboard.h>
#include <PFigure.h>
#include <PPoint.h>

using namespace std;

TEST_CASE_METHOD(PCheckboard, "Test the 'find available moves' system", "[checkboard-path]") {
	WHEN("There is one king in the middle of a map") {
		auto king = make_shared<PFigure>(PPoint(3,3), FigureType::King, FigurePlayer::Whites);

		auto path = buildPath(*king);

		THEN ("Can move everywhere") {
			REQUIRE(path.size() == 8); // top topRight right rightBot bot botLeft left leftTop
		}
	}
	WHEN ("King moved to the top right corner") {
		auto king = make_shared<PFigure>(PPoint(7,7), FigureType::King, FigurePlayer::Whites);
		auto path = buildPath(*king);

		THEN("Can move only in three directions left-down'wards") {
			REQUIRE(path.size() == 3); // bot botLeft left
		}
	}

}
