#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;
constexpr Uint32 FRAME_DELAY = 16; // 약 60 FPS

// 🆕 NEW: 함수 원형 선언
bool initializeGame(SDL_Window** window, SDL_Renderer** renderer);
void gameLoop(SDL_Renderer* renderer);
bool processInput(void);
void renderFrame(SDL_Renderer* renderer);
void shutdownGame(SDL_Window* window, SDL_Renderer* renderer);

int main(int argc, char* argv[])
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!initializeGame(&window, &renderer)) {
        return 1;
    }

    gameLoop(renderer);
    shutdownGame(window, renderer);

    return 0;
}

bool initializeGame(SDL_Window** window, SDL_Renderer** renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL 초기화 실패: %s\n", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow(
        "SDLGameBasic",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!*window) {
        fprintf(stderr, "윈도우 생성 실패: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        fprintf(stderr, "렌더러 생성 실패: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    return true;
}

void gameLoop(SDL_Renderer* renderer) {
    bool running = true;
    while (running) {
        running = processInput();
        renderFrame(renderer);
        SDL_Delay(FRAME_DELAY);
    }
}

bool processInput(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            return false;
        }
    }
    return true;
}

void renderFrame(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // 🆕 NEW: 파란색 배경
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void shutdownGame(SDL_Window* window, SDL_Renderer* renderer) {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}