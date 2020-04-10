//
// Created by Artem Martus on 06.04.2020.
//

#include "PGame.h"
#include "PViewSide.h"
#include "PPoint.h"
#include "PFigure.h"
#include "PCheckboard.h"
#include "PSaver.h"

#include <cstdlib>
#include <set>
#include <list>
#include <utility>

using namespace std;


PGame::PGame(shared_ptr<PViewSide> v, shared_ptr<PSaver> s) noexcept
		: view(std::move(v)), saver(std::move(s)) {
	checkboard = make_shared<PCheckboard>();
}

bool PGame::run() {
	checkboard->initialize();

	while (!checkboard->onePlayerLeft()) {
		view->renderFigures(checkboard);

		auto availableMoves = checkboard->canMoveFrom(checkboard->getWhitesTurn() ? Whites : Blacks);
		if (availableMoves.empty())
			break;

		static const list<string> actions = {"Move", "Save", "Load", "Restart", "Quit"};
		auto response = view->askForAction(checkboard->getWhitesTurn(), actions);
		switch (response) {
			case 1:
				try {
					saver->saveCheckboard(checkboard);
					view->renderText("Game saved");
				} catch (std::exception &e) {
					view->renderText("Couldn't save the game :(");
					view->renderText(e.what());
				}
				continue;
			case 0: {
				set<shared_ptr<PFigure>> freeFigures;
				for (const auto &i: availableMoves)
					freeFigures.insert(i.first);

				view->renderFreeFigures(freeFigures);
				auto figure = selectFigure(freeFigures);

				list<shared_ptr<PPoint>> path;
				for (const auto &i: availableMoves)
					if (*i.first == *figure && *(i.first->getPoint()) == *(figure->getPoint()))
						path.push_back(i.second);


				while (path.empty()) {
					view->renderText("No possible turns, select another figure");
					figure = selectFigure(freeFigures);
					for (const auto &i: availableMoves)
						if (*i.first == *figure && *(i.first->getPoint()) == *(figure->getPoint()))
							path.push_back(i.second);
				}

				auto from = figure->getPoint();
				view->renderSelectedInfo(figure);
				view->renderMayGoToPath(path);

				auto to = view->getPoint("Enter point to where we move: (0-7 0-7)");
				auto possibleFigure = checkboard->at(to);
				while (!checkboard->prepareMove(from, to)) {
					view->renderText("Cannot move to that point! try another");
					to = view->getPoint("to where we move: (0-7 0-7)");
					possibleFigure = checkboard->at(to);
				}
				if (possibleFigure)
					view->renderKillText(possibleFigure->asChar(), figure->asChar());
				else
					view->renderText("Move completed");

			}
				break;
			case 2:
				try {
					auto file = saver->loadCheckboard();
					checkboard = file;
					view->renderText("Game loaded");
				} catch (std::exception &e) {
					view->renderText("Couldn't restore the game :(");
					view->renderText(e.what());
				}

				continue;
			case 3:
				view->renderText("Game restarted");
				checkboard->initialize();
				return run();
			case 4:
				break;
			default:
				throw runtime_error("how could you even get here????");
		}

		checkboard->setTurn(!checkboard->getWhitesTurn());
	}
	return !checkboard->getWhitesTurn();
}

PGame::~PGame() {
	checkboard = nullptr;
}


std::shared_ptr<PFigure> PGame::selectFigure(const set<shared_ptr<PFigure>> &allowed) {
	auto from = view->getPoint("Enter point from where to move: (0-7 0-7)");
	auto figure = checkboard->at(from);

	auto ally = [&](const shared_ptr<PFigure> &f) -> bool {
		bool t = checkboard->getWhitesTurn();
		return !((f->getPlayer() == FigurePlayer::Whites && !t)
		         || (f->getPlayer() == FigurePlayer::Blacks && t));
	};

	auto good = [&](const shared_ptr<PFigure> &f) -> bool {
		for (const auto &i: allowed)
			if (*i == *f)
				if (*i->getPoint() == *f->getPoint())
					return true;
		return false;
	};

	while (!figure && !ally(figure) && !good(figure)) {
		view->renderText("No suitable ally figures found at specified point, try again");
		from = view->getPoint("from where to move: (0-7 0-7)");
		figure = checkboard->at(from);
	}

	return figure;
}
