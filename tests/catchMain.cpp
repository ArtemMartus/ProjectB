//
// Created by Artem Martus on 08.04.2020.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

// this is needed as testing entry point

TEST_CASE("DELETING NULLPTR IS SAFE") {
	int * ptr = nullptr;
	delete ptr;
}