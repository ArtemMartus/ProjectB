//
// Created by Artem Martus on 07.04.2020.
//

#include <Checkboard.h>
#include <Figure.h>
#include <Point.h>
#include <Saver.h>

#include <fstream>
#include <sstream>

using namespace std;

shared_ptr<Checkboard> Saver::loadCheckboard() const
{
    ifstream file(fileName);
    if (!file.is_open()) // stream failed to read int data
        throw runtime_error("Couldn't open savefile");

    list<shared_ptr<Figure>> objects;
    string str;
    getline(file, str);

    bool turn = atoi(str.c_str());
    getline(file, str);

    while (!file.eof()) {
        objects.push_back(restoreFigure(str));
        getline(file, str);
    }
    auto c = make_shared<Checkboard>(objects);
    c->setTurn(turn);
    return c;
}

void Saver::saveCheckboard(const shared_ptr<Checkboard>& checkboard) const
{
    if (!checkboard)
        return;

    ofstream file(fileName);
    if (!file.is_open()) // stream failed to read int data
        throw runtime_error("Couldn't write to savefile");

    file << checkboard->getWhitesTurn() << "\n";
    for (const auto& item : checkboard->getAllFigures())
        file << dumpFigure(item) << "\n";
    file.close();
}

string Saver::dumpFigure(const shared_ptr<Figure>& fig) const
{
    if (!fig)
        throw invalid_argument("Cannot dump null figure");
    ostringstream stream;
    stream << fig->getPlayer() << " " << fig->getType() << " "
           << fig->getPoint()->getX() << " " << fig->getPoint()->getY() << " "
           << fig->getMovesCount();

    if (fig->isAlive())
        stream << " " << -1;
    else
        stream << " " << 1 << " " << dumpFigure(fig->getKilledBy());

    return stream.str();
}

shared_ptr<Figure> Saver::restoreFigure(const std::string& data) const
{
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

    if (!stream.eof() && stream.fail()) // stream failed to read int data
        throw runtime_error("Got bad formatted savefile");

    auto figure = make_shared<Figure>(Point(x, y), type, player, moves);

    if (i == 1) {
        string killerInfo;
        getline(stream, killerInfo);
        figure->isCapturedBy(restoreFigure(killerInfo));
    }
    return figure;
}

Saver::Saver(string f) noexcept
    : fileName(std::move(f))
{
}
