#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include "TextureManager.h"
#include "GameObject.h"     
#include "Player.h"         
#include "Enemy.h"          
#include <iostream>

class Game {
public:
    // 🆕 NEW: 싱글톤 패턴을 위한 정적 인스턴스 접근 메서드
    static Game* Instance() {
        if (s_pInstance == nullptr) {
            s_pInstance = new Game();       // 지연 초기화
        }
        return s_pInstance;
    }

    bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
    void gameLoop();
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() const;

private:
    // 🔄 CHANGE: 생성자와 소멸자를 private으로 변경
    Game();
    ~Game();

    // 🆕 NEW: 복사 생성자와 대입 연산자 삭제 (C++11 이상)
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    // 🆕 NEW: 정적 포인터 선언
    static Game* s_pInstance;

    bool m_bRunning;
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

    const int TARGET_FPS = 60;
    const int FRAME_DELAY = 1000 / TARGET_FPS;
    Uint32 m_frameStart;
    int m_frameTime;
    int m_frameCount;
    Uint32 m_lastTime;

    std::vector<GameObject*> m_gameObjects;
};

// 🆕 NEW: Game 클래스에 대한 별칭 정의
typedef Game TheGame;

#endif // GAME_H