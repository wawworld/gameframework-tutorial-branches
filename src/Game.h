#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>               // 🆕 NEW: STL vector 헤더 추가
#include "TextureManager.h"
#include "GameObject.h"     
#include "Player.h"         
#include "Enemy.h"          
#include <iostream>

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
    void gameLoop();
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() const;

private:
    bool m_bRunning;
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

    // FPS 관련 멤버 변수
    const int TARGET_FPS = 60;
    const int FRAME_DELAY = 1000 / TARGET_FPS;
    Uint32 m_frameStart;
    int m_frameTime;
    int m_frameCount;
    Uint32 m_lastTime;

    // 🔄 CHANGE: 개별 포인터에서 벡터로 변경
    std::vector<GameObject*> m_gameObjects;  // 🆕 NEW: 모든 게임 객체를 담는 컨테이너
};

#endif // GAME_H