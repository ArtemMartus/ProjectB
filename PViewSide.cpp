//
// Created by Artem Martus on 06.04.2020.
//

#include "PViewSide.h"
#include "PCheckboard.h"
#include "PFigure.h"
#include <iostream>
#include <iomanip>
#include <iterator>
#include <algorithm>

using namespace std;

void PViewSide::renderText(std::string str) {
	cout << str << endl;
}

void PViewSide::renderText(char ch) {
	cout << ch << endl;
}

void PViewSide::renderFigures(PCheckboard *board) {
	auto point = new PPoint(0, 0);
	PFigure *figure = nullptr;
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
			figure = board->at(point);
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
	delete point;
}

int PViewSide::askForAction(bool whitesTurn, const list<string> &actions) {
	cout << (whitesTurn ? "Whites," : "Blacks,") << " your action: ";
	int index = 0;
	for (const auto &item: actions) {
		cout << index++ << " - " << item;
		if (index < actions.size())
			cout << ", ";
	}
	cout << endl;

	return inputAction(0, actions.size() - 1);
}

int PViewSide::inputAction(int lowBounds, int highBounds) {
	int i = -1;
	cin >> i;
	while (!cin.good())
	{
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

PPoint *PViewSide::getPoint(const std::string &message) {
	unsigned int x, y;
	cout << message << endl;
	x = inputAction(0, 7);
	y = inputAction(0, 7);
	return new PPoint(x, y);
}

void PViewSide::renderKillText(char i, char i1) {
	cout << i << " got killed by " << i1 << endl;
}

void PViewSide::renderSelectedInfo(PFigure *pFigure) {
	cout << "Selected " << pFigure->asChar() << " of "
	     << (pFigure->getPlayer() == PFigure::Player::Whites ? "Whites" : "Blacks")
	     << " at " << pFigure->getPoint()->asString() << endl;
}

void PViewSide::renderMayGoToPath(const std::list<PPoint *>& list) {
	cout << "May go to following points: ";
	int index = 0;
	for (auto i: list) {
		cout << i->asString();
		index++;
		if (index < list.size())
			cout << ", ";
	}
	cout << endl;
}
