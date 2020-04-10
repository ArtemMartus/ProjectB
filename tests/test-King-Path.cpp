//
// Created by Artem Martus on 09.04.2020.
//


#include "Tests.hpp"

TEST_CASE_METHOD(PPathSystem, "Test the king movement system") {
	WHEN("There is one king in the middle of a map") {
		auto king = make_shared<PFigure>(PPoint(3, 3), FigureType::King, FigurePlayer::Whites);
		setBoard({king});
		auto path = buildPath(king);

		THEN ("Can move everywhere") {
			REQUIRE(path.size() == 8); // top topRight right rightBot bot botLeft left leftTop
		}
	} WHEN ("King moved to the top right corner") {
		auto king = make_shared<PFigure>(PPoint(7, 7), FigureType::King, FigurePlayer::Whites);
		setBoard({king});
		auto path = buildPath(king);

		THEN("Can move only in three directions left-down'wards") {
			REQUIRE(path.size() == 3); // bot botLeft left
		}

	} WHEN ("King stands on a line with enemy rook") {
		auto king = make_shared<PFigure>(PPoint(7, 7), FigureType::King, FigurePlayer::Whites);
		auto enemyKing = make_shared<PFigure>(PPoint(0, 0), FigureType::King, FigurePlayer::Blacks);
		auto enemyRook = make_shared<PFigure>(PPoint(0, 7), FigureType::Rook, FigurePlayer::Blacks);
		setBoard({king, enemyRook, enemyKing});

		THEN("King can have only two options of where to move") {
			auto moves = getListOfAvailableMoves(king->getPlayer());
			list<shared_ptr<PPoint>> kingMoves;
			for (const auto &i: moves)
				if (*i.first == *king)
					kingMoves.push_back(i.second);

			REQUIRE(kingMoves.size() == 2);
		}

	} WHEN ("King stands on a line with enemy rook and ally bishop can kill enemy rook") {
		auto king = make_shared<PFigure>(PPoint(7, 7), FigureType::King, FigurePlayer::Whites);
		auto enemyKing = make_shared<PFigure>(PPoint(0, 0), FigureType::King, FigurePlayer::Blacks);
		auto enemyRook = make_shared<PFigure>(PPoint(0, 7), FigureType::Rook, FigurePlayer::Blacks);
		auto allyBishop = make_shared<PFigure>(PPoint(7, 0), FigureType::Bishop, FigurePlayer::Whites);
		setBoard({king, enemyRook, allyBishop, enemyKing});


		THEN("Can move with a king or use bishop to kill enemy") {
			auto moves = getListOfAvailableMoves(FigurePlayer::Whites);
			list<shared_ptr<PPoint>> kingMoves;
			list<shared_ptr<PPoint>> bishopMoves;
			for (const auto &i: moves)
				if (*i.first == *king)
					kingMoves.push_back(i.second);
				else if (*i.first == *allyBishop)
					bishopMoves.push_back(i.second);

			REQUIRE(kingMoves.size() == 2); // bot botLeft
			REQUIRE(moves.size() == 3); // king and the allyBishop
			REQUIRE(bishopMoves.size() == 1); // only one turn to get that rook
		}
	}
}
