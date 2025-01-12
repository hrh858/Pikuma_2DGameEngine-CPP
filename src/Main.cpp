#include "./Game/Game.h"

int main(int argc, char* argv[]) {
    Game game;

    game.Initialize(120);
    game.Run();
    game.Destroy();

    return 0;
}
