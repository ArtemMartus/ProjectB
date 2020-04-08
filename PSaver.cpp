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
        if (!file.is_open()) { // stream failed to read int data
            throw runtime_error("Couldn't open savefile");
            return nullptr;
        }
	list<PFigure*> objects;
	string str;
	getline(file,str);

	bool turn = atoi(str.c_str());

	while (!file.eof()) {
		getline(file,str);
		objects.push_back(restorePFigure(str));
	}
	auto c = new PCheckboard(objects);
	c->setTurn(turn);
	return c;
}

void PSaver::saveCheckboard(PCheckboard *checkboard) {
	ofstream file(fileName);
        if (!file.is_open()) { // stream failed to read int data
            throw runtime_error("Couldn't write to savefile");
            return;
        }
	file << checkboard->getWhitesTurn() << "\n";
	for (auto item: checkboard->getAllFigures())
		file << dumpPFigure(item) << "\n";
	file.close();
}

std::string PSaver::dumpPFigure(PFigure *fig) {
	if (!fig) throw std::invalid_argument("Cannot dump null figure");
	ostringstream stream;
	stream << fig->getPlayer()
	       << " " << fig->getType()
	       << " " << fig->getPoint()->getX()
	       << " " << fig->getPoint()->getY();

	if (fig->isAlive())
		stream << " " << -1;
	else
		stream << " " << 1 << " " << dumpPFigure(fig->getKilledBy());

	return stream.str();
}

PFigure *PSaver::restorePFigure(const std::string &data) {
	istringstream stream(data);
        PFigure::Player player = PFigure::Player::Whites;
        PFigure::Type type = PFigure::Type::Pawn;
        unsigned int x = 0, y = 0;

        int i = 0;

	stream >> i;
	player = static_cast<PFigure::Player>(i);
	stream >> i;
	type = static_cast<PFigure::Type>(i);
        stream >> x >> y; // i indicates -1 = alive, 1 = killed by ...

        stream >> i;

        if (!stream.eof() && stream.fail()) { // stream failed to read int data
            throw runtime_error("Got bad formatted savefile");
            return nullptr;
        }

	auto figure = new PFigure(new PPoint(x, y), type, player);

	if (i == 1) {
		string data2;
		getline(stream,data2);
		auto killedBy = restorePFigure(data2);
		figure->kill(killedBy);
		delete killedBy;
	}
	return figure;
}

PSaver::PSaver(std::string f): fileName(f) {
}
