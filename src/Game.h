#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <iostream>

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
    void gameLoop();    // 🆕 NEW: 게임 루프 메서드
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

    // 🆕 NEW: FPS 관련 멤버 변수
    const int TARGET_FPS = 60;
    const int FRAME_DELAY = 1000 / TARGET_FPS;
    Uint32 m_frameStart;
    int m_frameTime;
    int m_frameCount;
    Uint32 m_lastTime;

    // 🆕 NEW: 텍스처 위치 관리
    SDL_Rect m_dstRect;
};

#endif // GAME_H