#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>  // 🆕 NEW: SDL_Image 헤더 추가
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
    SDL_Texture* m_pTexture;

    // 🆕 NEW: 두 번째 텍스처 관련 멤버 변수
    SDL_Texture* m_pTexture2;
    SDL_Rect m_srcRect2;
    SDL_Rect m_destRect2;

    // FPS 관련 멤버 변수
    const int TARGET_FPS = 60;
    const int FRAME_DELAY = 1000 / TARGET_FPS;
    Uint32 m_frameStart;
    int m_frameTime;
    int m_frameCount;
    Uint32 m_lastTime;

    // 애니메이션 관련 멤버 변수
    SDL_Rect m_srcRect;
    SDL_Rect m_destRect;
    int m_direction;

    // 🆕 NEW: 키보드 입력 처리 멤버 변수
    int m_velocityX;
    int m_velocityY;
};

#endif // GAME_H