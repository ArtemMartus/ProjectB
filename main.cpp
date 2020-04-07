#include "PGame.h"
#include "PViewSide.h"
#include "PSaver.h"


int main() {
	auto view = new PViewSide();
	auto saver = new PSaver("./saveFile.txt");
	PGame game(view, saver);
	bool whiteWon = game.run();
	if (whiteWon) {
		view->renderText("Whites won, congratulations!");
	} else {
		view->renderText("Blacks won, congratulations!");
	}

	delete view;
	delete saver;
	return 0;
}
