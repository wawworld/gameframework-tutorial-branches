#include "Game.h"
#include <iostream>

// 상수 선언
const int WINDOW_POS_X = 100;
const int WINDOW_POS_Y = 100;
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int main(int argc, char* argv[])
{
    Game* g_game = new Game();

    if (g_game->init("Sorc Box & Dest Box", WINDOW_POS_X, WINDOW_POS_Y,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN)) {
        g_game->gameLoop();    // 🆕 NEW: 게임 루프 실행
    }
    else {
        std::cerr << "게임 초기화 실패." << std::endl;
        delete g_game;
        return 1;
    }

    delete g_game;
    return 0;
}