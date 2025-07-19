#include "Game.h"

int main(int argc, char* argv[]) {
    // 게임 인스턴스 초기화
    if (TheGame::Instance()->init("Component Sprite Rendering", 100, 100, 800, 600, SDL_WINDOW_SHOWN)) {
        // 게임 루프 실행
        TheGame::Instance()->gameLoop();
    } else {
        std::cerr << "게임 초기화 실패." << std::endl;
        return 1;
    }
    
    // 게임 정리
    TheGame::Instance()->clean();
    
    return 0;
}