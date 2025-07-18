#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
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

    // FPS 관련 멤버 변수
    const int TARGET_FPS = 60;
    const int FRAME_DELAY = 1000 / TARGET_FPS;
    Uint32 m_frameStart;
    int m_frameTime;
    int m_frameCount;
    Uint32 m_lastTime;

    // 🆕 NEW: 애니메이션 관련 멤버 변수
    SDL_Rect m_srcRect;      // 원본 상자 (스프라이트 시트 내 프레임 위치)
    SDL_Rect m_destRect;     // 대상 상자 (화면 출력 위치)
    int m_direction;         // 이동 방향 (1: 오른쪽, -1: 왼쪽)
};

#endif // GAME_H