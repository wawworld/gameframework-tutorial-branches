#include "Game.h"
#include <iostream>

// 상수 선언
const int WINDOW_POS_X = 100;      // 창의 X 위치
const int WINDOW_POS_Y = 100;      // 창의 Y 위치
const int WINDOW_WIDTH = 640;      // 창의 너비
const int WINDOW_HEIGHT = 480;     // 창의 높이

int main(int argc, char* argv[]) {
    Game* g_game = new Game();  // Game 객체 생성

    if (g_game->init("SDLGameClass", WINDOW_POS_X, WINDOW_POS_Y,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN)) {
        while (g_game->running()) {
            g_game->handleEvents();    // 이벤트 처리
            g_game->update();          // 게임 상태 업데이트
            g_game->render();          // 화면 렌더링
        }
    }
    else {
        std::cerr << "게임 초기화 실패." << std::endl;
        delete g_game;
        return 1;
    }

    delete g_game;
    return 0;
}