#include "PGame.h"
#include "PViewSide.h"
#include "PSaver.h"

#include <memory>

using std::make_shared;

int main() {
	auto view = make_shared<PViewSide>(); // this class is responsible for rendering and input, aka 'view' side
	auto saver = make_shared<PSaver>("./saveFile.txt"); // this class operates with files
	PGame game(view, saver); // this is our controller which composes view and business logic together
	bool whiteWon = game.run();
	if (whiteWon) {
		view->renderText("Whites won, congratulations!");
	} else {
		view->renderText("Blacks won, congratulations!");
	}
	
	return 0;
}
