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
