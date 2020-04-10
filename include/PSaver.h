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

	virtual std::string dumpPFigure(const std::shared_ptr<PFigure> &fig) const;

	virtual std::shared_ptr<PFigure> restorePFigure(const std::string &data) const;

public:
	explicit PSaver(std::string filename) noexcept;

	virtual void saveCheckboard(const std::shared_ptr<PCheckboard> &checkboard) const;

	virtual std::shared_ptr<PCheckboard> loadCheckboard() const;
};


#endif //PROJECTB_PSAVER_H
