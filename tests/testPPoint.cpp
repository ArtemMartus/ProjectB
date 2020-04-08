//
// Created by Artem Martus on 08.04.2020.
//

#include "catch.hpp"

#include <PPoint.h>

SCENARIO("Point coordinates can be accessed via getters and setters", "[PPoint]") {
	GIVEN("A PPoint with 0 0 coordinates") {
		PPoint point(0, 0);
		REQUIRE(point.getX() == 0);
		REQUIRE(point.getY() == 0);
		REQUIRE(point.inBounds());

		WHEN("We change x coordinate to 5") {
			point.setX(5);
			THEN("Coordinate updated and getter returns 5") {
				REQUIRE(point.getX() == 5);
			}
		} WHEN("We change y coordinate to 5") {
			point.setY(5);
			THEN("Coordinate updated and getter returns 5") {
				REQUIRE(point.getY() == 5);
			}
		} WHEN("We get x and y propery and store it in variables") {
			auto x = point.getX();
			auto y = point.getY();
			THEN("Future gets return same values, so getters are const functions") {
				REQUIRE(point.getX() == x);
				REQUIRE(point.getY() == y);
			}
		}
	}

	GIVEN("Point with coordinates 4 4") {
		PPoint point(4, 4);
		REQUIRE(point.inBounds());

		WHEN("We call its asString() method") {
			auto str = point.asString();
			THEN("We get following string '(4,4)'") {
				REQUIRE(str == "(4, 4)");
			}
		}

		WHEN("We create another point with 4 4 coordinates and call isEquals() method") {
			PPoint point2(4, 4);
			REQUIRE(point2.inBounds());

			THEN("We got correct point comparison") {
				REQUIRE(point.isEquals(&point2));
			}
		} AND_WHEN("We create another point with 3 4 coordinates and call isEquals() method") {
			PPoint point2(3, 4);
			THEN("We got correct point comparison") {
				REQUIRE(!point.isEquals(&point2));
			}
		}
	}

	GIVEN("A point with -1 -1 coordinates") {
		PPoint point(-1, -1);
		REQUIRE(!point.inBounds());

		WHEN("We get them coordinates") {
			auto x = point.getX();
			auto y = point.getY();
			THEN("We realize they both > 0 because chessboard have no negative coordinates") {
				REQUIRE(x > 0);
				REQUIRE(y > 0);
			}
		}
	}
}