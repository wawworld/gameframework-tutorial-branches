#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;
constexpr Uint32 DELAY_TIME = 5000;

int main(int argc, char* argv[])
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // SDL 초기화
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL 초기화 실패: %s\n", SDL_GetError());
        return 1;
    }

    // SDL 윈도우 생성
    window = SDL_CreateWindow(
        "HelloSDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        fprintf(stderr, "윈도우 생성 실패: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 렌더러 생성
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (renderer == nullptr) {
        fprintf(stderr, "렌더러 생성 실패: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 화면 렌더링
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 🆕 NEW: 검은색 배경
    SDL_RenderClear(renderer);                       // 🆕 NEW: 화면 지우기
    SDL_RenderPresent(renderer);                     // 🆕 NEW: 화면 표시

    // 지연 및 종료 처리
    SDL_Delay(DELAY_TIME);

    // 리소스 해제
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}