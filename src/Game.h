#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "TextureManager.h"  // 🆕 NEW: TextureManager 헤더 추가
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

    // 게임 오브젝트 위치 및 이동
    SDL_Rect m_srcRect;
    SDL_Rect m_destRect;
    SDL_Rect m_srcRect2;
    SDL_Rect m_destRect2;
    int m_direction;
    int m_velocityX;
    int m_velocityY;

    // 🆕 NEW: 점프 관련 멤버 변수
    bool m_isJumping;
    int m_jumpStartY;
    int m_jumpHeight;

    // 🆕 NEW: 애니메이션 관련 변수
    int m_currentFrame;
    Uint32 m_lastFrameTime;
};

#endif // GAME_H