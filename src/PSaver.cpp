//
// Created by Artem Martus on 07.04.2020.
//

#include "PSaver.h"
#include "PCheckboard.h"
#include "PFigure.h"
#include "PPoint.h"

#include <fstream>
#include <sstream>

using namespace std;

PCheckboard *PSaver::loadCheckboard() {
	ifstream file(fileName);
	if (!file.is_open())  // stream failed to read int data
		throw runtime_error("Couldn't open savefile");

	list<shared_ptr<PFigure>> objects;
	string str;
	getline(file, str);

	bool turn = atoi(str.c_str());
	getline(file, str);

	while (!file.eof()) {
		objects.push_back(restorePFigure(str));
		getline(file, str);
	}
	auto c = new PCheckboard(objects);
	c->setTurn(turn);
	return c;
}

void PSaver::saveCheckboard(PCheckboard *checkboard) {
	ofstream file(fileName);
	if (!file.is_open())  // stream failed to read int data
		throw runtime_error("Couldn't write to savefile");

	file << checkboard->getWhitesTurn() << "\n";
	for (auto item: checkboard->getAllFigures())
		file << dumpPFigure(item) << "\n";
	file.close();
}

string PSaver::dumpPFigure(const shared_ptr<PFigure> &fig) {
	if (!fig) throw invalid_argument("Cannot dump null figure");
	ostringstream stream;
	stream << fig->getPlayer()
	       << " " << fig->getType()
	       << " " << fig->getPoint()->getX()
	       << " " << fig->getPoint()->getY()
	       << " " << fig->getMovesCount();

	if (fig->isAlive())
		stream << " " << -1;
	else
		stream << " " << 1 << " " << dumpPFigure(fig->getKilledBy());

	return stream.str();
}

shared_ptr<PFigure> PSaver::restorePFigure(const std::string &data) {
	istringstream stream(data);
	auto player = FigurePlayer::Whites;
	auto type = FigureType::Pawn;
	unsigned int x = 0, y = 0, moves = 0;

	int i = 0;

	stream >> i;
	player = static_cast<FigurePlayer>(i);
	stream >> i;
	type = static_cast<FigureType>(i);
	stream >> x >> y >> moves >> i; // i indicates -1 = alive, 1 = killed by ...

	if (!stream.eof() && stream.fail())  // stream failed to read int data
		throw runtime_error("Got bad formatted savefile");


	auto figure = make_shared<PFigure>(PPoint(x, y), type, player, moves);

	if (i == 1) {
		string killerInfo;
		getline(stream, killerInfo);
		figure->capture(restorePFigure(killerInfo));
	}
	return figure;
}

PSaver::PSaver(string f) : fileName(f) {
}
