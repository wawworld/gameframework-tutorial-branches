#include "Game.h"

int main(int argc, char* argv[]) {
    // TheGame::Instance()를 통한 싱글톤 접근
    if (TheGame::Instance()->init("Game Window", 100, 100, 800, 600, SDL_WINDOW_SHOWN)) {
        // 동일한 인스턴스를 통한 게임 루프 실행
        TheGame::Instance()->gameLoop();
    }
    else {
        std::cerr << "게임 초기화 실패." << std::endl;
        return 1;
    }

    // 리소스 정리도 동일한 인스턴스 사용
    TheGame::Instance()->clean();

    return 0;
}