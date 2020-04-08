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
#include <list>

using std::string;
using std::list;


PGame::PGame(PViewSide *v, PSaver *s) : view(v), saver(s) {
	checkboard = new PCheckboard();
}

bool PGame::run() {
	// who start?
	checkboard->setTurn(true);

	while (!checkboard->onePlayerLeft()) {

		view->renderFigures(checkboard);
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

				auto figure = selectFigure();
				auto path = checkboard->buildPath(figure);
				while (path.empty()) {
					view->renderText("No possible turns, select another figure");
					figure = selectFigure();
					path = checkboard->buildPath(figure);
				}

				auto from = figure->getPoint();
				view->renderSelectedInfo(figure);
				view->renderMayGoToPath(path);
				for (auto i: path)
					delete i;
				path.clear();

				PPoint *to = view->getPoint("Enter point to where we move: (0-7 0-7)");
				auto possibleFigure = checkboard->at(to);
				while (!checkboard->move(from, to)) {
					delete to;
					view->renderText("Cannot move to that point! try another");
					to = view->getPoint("to where we move: (0-7 0-7)");
					possibleFigure = checkboard->at(to);
				}
				if (possibleFigure)
					view->renderKillText(possibleFigure->asChar(), figure->asChar());
				else
					view->renderText("Move completed");

				delete to;
			}
				break;
			case 2:
				try {
					auto file = saver->loadCheckboard();
					delete checkboard;
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
				return !checkboard->getWhitesTurn();
		}

		checkboard->setTurn(!checkboard->getWhitesTurn());
	}
	return false;
}

PGame::~PGame() {
	if (checkboard) {
		delete checkboard;
		checkboard = nullptr;
	}
}


PFigure *PGame::selectFigure() {
	PPoint *from = view->getPoint("Enter point from where to move: (0-7 0-7)");
	PFigure *figure = checkboard->at(from);

	auto ally = [&](PFigure *f) -> bool {
		bool t = checkboard->getWhitesTurn();
		if (!f) return false;
		return !((f->getPlayer() == FigurePlayer::Whites && !t)
		         || (f->getPlayer() == FigurePlayer::Blacks && t));
	};

	while (!ally(figure)) {
		delete from;
		view->renderText("No ally figures found at specified point, try again");
		from = view->getPoint("from where to move: (0-7 0-7)");
		figure = checkboard->at(from);
	}
	delete from;

	return figure;
}
