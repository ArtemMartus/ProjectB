//
// Created by Artem Martus on 06.04.2020.
//

#include <Game.h>
#include <Saver.h>
#include <ViewSide.h>

#include <memory>

using std::make_shared;

int main()
{
    auto view = make_shared<ViewSide>(); // this class is responsible for
    // rendering and input, aka 'view' side
    auto saver = make_shared<Saver>("./saveFile.txt"); // this class operates with files
    Game game(view, saver); // this is our controller which composes view and
    // business logic together
    bool whiteWon = game.run();
    if (whiteWon) {
        view->renderText("Whites won, congratulations!");
    } else {
        view->renderText("Blacks won, congratulations!");
    }

    return 0;
}
