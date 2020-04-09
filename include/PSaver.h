//
// Created by Artem Martus on 07.04.2020.
//

#ifndef PROJECTB_PSAVER_H
#define PROJECTB_PSAVER_H

#include <string>
#include <memory>

class PCheckboard;

class PFigure;

class PSaver {
	std::string fileName;

	std::string dumpPFigure(const std::shared_ptr<PFigure> &fig);

	virtual std::shared_ptr<PFigure> restorePFigure(const std::string &data);

public:
	explicit PSaver(std::string filename);

	void saveCheckboard(PCheckboard *checkboard);

	PCheckboard *loadCheckboard();
};


#endif //PROJECTB_PSAVER_H
