#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include "GameObject.h"
#include <iostream>


class Game {
public:
    static Game* Instance() {
        if (s_pInstance == nullptr) {
            s_pInstance = new Game();
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
    Game();
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    static Game* s_pInstance;

    bool m_bRunning;
    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

    const int TARGET_FPS = 60;
    const int FRAME_DELAY = 1000 / TARGET_FPS;

    int m_frameCount;

    std::vector<std::shared_ptr<GameObject>> m_gameObjects;

    void createGameObjects();
    void createPlayer();
    void createEnemies();
    void loadTextures();
    void testSpriteRendering();
    void resetGameObjects();  // 🆕 NEW: 리셋 기능
};

typedef Game TheGame;

#endif // GAME_H