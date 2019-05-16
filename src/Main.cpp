#include "Game.h"
#include "TitleState.h"

int main(int argc, char** argv) {
	Game& game = Game::GetInstance();
	TitleState* init = new TitleState();

	game.Push(init);
	game.Run();

	return 0;
}
