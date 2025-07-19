#include "Game.h"

int main(int argc, char* argv[]) {
    if (TheGame::Instance()->init("Component Behavior Control", 100, 100, 800, 600, SDL_WINDOW_SHOWN)) {
        TheGame::Instance()->gameLoop();
    } else {
        std::cerr << "게임 초기화 실패." << std::endl;
        return 1;
    }

    TheGame::Instance()->clean();

    return 0;
}