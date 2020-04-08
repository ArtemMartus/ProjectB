#include "PGame.h"
#include "PViewSide.h"
#include "PSaver.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#endif

int main() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	SetConsoleCP(1251);
#endif

	auto view = new PViewSide(); // this class is responsible for rendering and input, aka 'view' side
	auto saver = new PSaver("./saveFile.txt"); // this class operates with files
	PGame game(view, saver); // this is our controller which composes view and business logic together
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
