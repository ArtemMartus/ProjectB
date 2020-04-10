//
// Created by Artem Martus on 06.04.2020.
//

#include "PViewSide.h"
#include "PCheckboard.h"
#include "PFigure.h"
#include "PPoint.h"
#include <iostream>
#include <iomanip>
#include <iterator>

#ifdef _WIN32

#include <algorithm>

#elif __linux__

#include <climits> ///INT_MAX for linux

#endif

#include <set>

using namespace std;

void PViewSide::renderText(const string &str) const {
	cout << str << endl;
}

void PViewSide::renderFigures(const shared_ptr<PCheckboard> &board) const {
	auto point = make_shared<PPoint>(0, 0);
	cout << "    ";
	for (int j = 0; j < 8; j++) {
		cout << setw(6) << j;
	}
	cout << "\n     -------------------------------------------------\n";
	for (int i = 7; i >= 0; i--) {
		cout << i << "  |";
		for (int j = 0; j < 8; j++) {
			char ch = '.';
			point->setX(j);
			point->setY(i);
			const auto figure = board->at(point);
			if (figure)
				ch = figure->asChar();
			cout << setw(6) << ch;
		}
		cout << "   |  " << i << "\n     -------------------------------------------------\n";
	}
	cout << "    ";
	for (int j = 0; j < 8; j++) {
		cout << setw(6) << j;
	}
	cout << endl << endl;
}

int PViewSide::askForAction(bool whitesTurn, const list<string> &actions) const {
	cout << (whitesTurn ? "Whites," : "Blacks,") << " your action: ";
	int index = 0;
	for (const auto &item: actions) {
		cout << index++ << " - " << item;
		if (index < actions.size())
			cout << ", ";
	}
	cout << endl;

	return inputAction(0, (int) actions.size() - 1);
}

int PViewSide::inputAction(int lowBounds, int highBounds) const {
	int i = -1;
	cin >> i;
	while (!cin.good()) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cerr << "Please make sure you are typing number, try again: ";
		cin >> i;
	}
	if (i < lowBounds || i > highBounds) {
		cerr << "Wrong input, try number between " << lowBounds << " and " << highBounds << endl;
		return inputAction(lowBounds, highBounds);
	}
	return i;
}

shared_ptr<PPoint> PViewSide::getPoint(const string &message) const {
	unsigned int x, y;
	cout << message << endl;
	x = inputAction(0, 7);
	y = inputAction(0, 7);
	return make_shared<PPoint>(x, y);
}

void PViewSide::renderKillText(char i, char i1) const {
	cout << i << " got killed by " << i1 << endl;
}

void PViewSide::renderSelectedInfo(const shared_ptr<PFigure> &pFigure) const {
	cout << "Selected " << pFigure->asChar() << " of "
	     << (pFigure->getPlayer() == FigurePlayer::Whites ? "Whites" : "Blacks")
	     << " at " << pFigure->getPoint()->asString() << endl;
}

void PViewSide::renderMayGoToPath(const list<shared_ptr<PPoint>> &list) const {
	cout << "May go to following points: ";
	int index = 0;
	for (const auto &i: list) {
		cout << *i;
		index++;
		if (index < list.size())
			cout << ", ";
	}
	cout << endl;
}

void PViewSide::renderFreeFigures(const set<std::shared_ptr<PFigure>> &set) const {
	cout << "May choose figures with following coordinates: ";
	int index = 0;
	for (const auto &i: set) {
		cout << *i->getPoint();
		index++;
		if (index < set.size())
			cout << ", ";
	}
	cout << endl;
}
