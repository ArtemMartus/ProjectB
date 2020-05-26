//
// Created by Artem Martus on 07.04.2020.
//

#pragma once

#include <memory>
#include <string>

class Checkboard;

class Figure;

class Saver {
    std::string fileName;

    std::string dumpFigure(const std::shared_ptr<Figure>& fig) const;
    std::shared_ptr<Figure> restoreFigure(const std::string& data) const;

public:
    explicit Saver(std::string filename) noexcept;

    void saveCheckboard(const std::shared_ptr<Checkboard>& checkboard) const;

    std::shared_ptr<Checkboard> loadCheckboard() const;
};
