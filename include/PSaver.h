//
// Created by Artem Martus on 07.04.2020.
//

#pragma once

#include <string>
#include <memory>

class PCheckboard;

class PFigure;

class PSaver {
    std::string fileName;

    std::string dumpPFigure(const std::shared_ptr<PFigure> &fig) const;
    std::shared_ptr<PFigure> restorePFigure(const std::string &data) const;

public:
    explicit PSaver(std::string filename) noexcept;

    void saveCheckboard(const std::shared_ptr<PCheckboard> &checkboard) const;

    std::shared_ptr<PCheckboard> loadCheckboard() const;
};
