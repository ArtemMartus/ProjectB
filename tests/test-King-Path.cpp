//
// Created by Artem Martus on 09.04.2020.
//


#include "Tests.hpp"

TEST_CASE_METHOD(PPathSystem, "Test the king movement system") {
	WHEN("Castling test") {
		auto king = PFigureFactory::buildKing(FigurePlayer::Whites);
		auto enemyKing = PFigureFactory::buildKing(FigurePlayer::Blacks);
		auto allyRooks = PFigureFactory::buildRooks(FigurePlayer::Whites);
		auto leftRook = allyRooks.front();
		auto rightRook = allyRooks.back();

		auto kingLeftRookPoint = PPoint(king->getX() - 2, king->getY());
		auto kingRightRookPoint = PPoint(king->getX() + 2, king->getY());

		setBoard({king, enemyKing, leftRook, rightRook});

		THEN("Can castle left and right") {
			REQUIRE(checkCastling(leftRook, king)); // castling method doesn't count future enemy checks
			REQUIRE(checkCastling(rightRook, king));
			auto moves = getListOfAvailableMoves(Whites);
			bool kingCastleLeft = false, kingCastleRight = false, leftRookCastle = false, rightRookCastle = false;
			for (const auto &i: moves) {
				if (*i.first == *king && *(i.first->getPoint()) == *(king->getPoint())) {
					if (*i.second == kingLeftRookPoint) kingCastleLeft = true;
					if (*i.second == kingRightRookPoint) kingCastleRight = true;
				} else if (*i.first == *leftRook && *(i.first->getPoint()) == *(leftRook->getPoint())) {
					if (*i.second == *king->getPoint()) leftRookCastle = true;
				} else if (*i.first == *rightRook && *(i.first->getPoint()) == *(rightRook->getPoint())) {
					if (*i.second == *king->getPoint()) rightRookCastle = true;
				}
			}
			REQUIRE(kingCastleLeft);
			REQUIRE(kingCastleRight);
			REQUIRE(leftRookCastle);
			REQUIRE(rightRookCastle);
		}

		AND_WHEN("We add enemy rook right above king coordinate at the end of left castling") {
			auto rook = make_shared<PFigure>(PPoint(king->getX() - 2, 5), Rook, Blacks);
			setBoard({king, enemyKing, leftRook, rightRook, rook});
			THEN("We cant castle to the left") {
				REQUIRE(checkCastling(leftRook, king));
				REQUIRE(checkCastling(rightRook, king));
				auto moves = getListOfAvailableMoves(Whites);

				bool kingCastleLeft = false, kingCastleRight = false, leftRookCastle = false, rightRookCastle = false;
				for (const auto &i: moves) {
					if (*i.first == *king && *(i.first->getPoint()) == *(king->getPoint())) {
						if (*i.second == kingLeftRookPoint) kingCastleLeft = true;
						if (*i.second == kingRightRookPoint) kingCastleRight = true;
					} else if (*i.first == *leftRook && *(i.first->getPoint()) == *(leftRook->getPoint())) {
						if (*i.second == *king->getPoint()) leftRookCastle = true;
					} else if (*i.first == *rightRook && *(i.first->getPoint()) == *(rightRook->getPoint())) {
						if (*i.second == *king->getPoint()) rightRookCastle = true;
					}
				}
				REQUIRE(!kingCastleLeft);
				REQUIRE(kingCastleRight);
				REQUIRE(!leftRookCastle);
				REQUIRE(rightRookCastle);
			}
		}

		AND_WHEN("We add enemy rook right above king coordinate at the end of right castling") {
			auto rook = make_shared<PFigure>(PPoint(king->getX() + 2, 5), Rook, Blacks);
			setBoard({king, enemyKing, leftRook, rightRook, rook});
			auto moves = getListOfAvailableMoves(Whites);
			bool kingCastleLeft = false, kingCastleRight = false, leftRookCastle = false, rightRookCastle = false;
			for (const auto &i: moves) {
				if (*i.first == *king && *(i.first->getPoint()) == *(king->getPoint())) {
					if (*i.second == kingLeftRookPoint) kingCastleLeft = true;
					if (*i.second == kingRightRookPoint) kingCastleRight = true;
				} else if (*i.first == *leftRook && *(i.first->getPoint()) == *(leftRook->getPoint())) {
					if (*i.second == *king->getPoint()) leftRookCastle = true;
				} else if (*i.first == *rightRook && *(i.first->getPoint()) == *(rightRook->getPoint())) {
					if (*i.second == *king->getPoint()) rightRookCastle = true;
				}
			}
			REQUIRE(kingCastleLeft);
			REQUIRE(!kingCastleRight);
			REQUIRE(leftRookCastle);
			REQUIRE(!rightRookCastle);
		}
	}

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
	} WHEN ("King stands near enemy queen") {
		auto king = make_shared<PFigure>(PPoint(7, 7), FigureType::King, FigurePlayer::Blacks);
		auto enemyKing = make_shared<PFigure>(PPoint(0, 0), FigureType::King, FigurePlayer::Whites);
		auto enemyQueen = make_shared<PFigure>(PPoint(6, 6), FigureType::Queen, FigurePlayer::Whites);
		setBoard({king, enemyQueen, enemyKing});


		THEN("The only turn is to capture the queen") {
			auto moves = getListOfAvailableMoves(FigurePlayer::Blacks);
			REQUIRE(moves.size() == 1); // king attacks queen
		}

		AND_WHEN("Somebody is protecting the queen - it's checkmate indeed") {
			auto pawn = make_shared<PFigure>(PPoint(5, 5), FigureType::Pawn, FigurePlayer::Whites);
			setBoard({king, enemyQueen, enemyKing, pawn});
			THEN ("No more moves for poor black king") {
				auto moves = getListOfAvailableMoves(FigurePlayer::Blacks);
				REQUIRE(moves.empty()); // checkmate situation
			}
		}
	}
}
