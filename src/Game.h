#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "TextureManager.h"
#include "GameObject.h"     // 🆕 NEW: GameObject 헤더 추가
#include "Player.h"         // 🆕 NEW: Player 헤더 추가  
#include "Enemy.h"          // 🆕 NEW: Enemy 헤더 추가
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

    // 🆕 NEW: 게임 객체 포인터들
    Player* m_pPlayer;              // 플레이어 객체 포인터
    Enemy* m_pEnemy;                // 적 객체 포인터
};

#endif // GAME_H