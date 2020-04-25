//
// Created by Artem Martus on 06.04.2020.
//

#pragma once

#include <memory>
#include <set>

class PViewSide;

class PCheckboard;

class PSaver;

class PPoint;

class PFigure;

class PGame {
    std::shared_ptr<PViewSide> view;
    std::shared_ptr<PSaver> saver;
    std::shared_ptr<PCheckboard> checkboard;

    std::shared_ptr<PFigure> selectFigure(
            const std::set<std::shared_ptr<PFigure>> &set);

public:
    PGame(std::shared_ptr<PViewSide> viewSide,
          std::shared_ptr<PSaver> saver) noexcept;

    ~PGame();

    bool run();
};
