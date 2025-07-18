#include "Game.h"
#include <iostream>

// 🆕 NEW: Game 클래스 생성자 - 멤버 변수 초기화
Game::Game() : m_bRunning(false), m_pWindow(nullptr), m_pRenderer(nullptr) {
    // 초기화 리스트를 통한 안전한 초기화
}

// 🆕 NEW: Game 클래스 소멸자 - 게임 종료 시 호출되어 리소스를 정리
Game::~Game() {
    clean();  // 모든 리소스를 정리하는 함수 호출
}

bool Game::init(const char* title, int xpos, int ypos,
    int width, int height, int flags) {
    // SDL 초기화
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // 창 생성
    m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (m_pWindow == nullptr) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // 렌더러 생성
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_pRenderer == nullptr) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_pWindow);
        SDL_Quit();
        return false;
    }

    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);  // 검은색 배경
    m_bRunning = true;  // 게임 실행 상태로 설정

    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_bRunning = false;
        }
    }
}

void Game::update() {
    // 🆕 NEW: 게임 로직 업데이트 (현재는 비어있음)
    // 향후 게임 오브젝트 위치, 상태 업데이트 등이 여기에 추가
}

void Game::render() {
    SDL_RenderClear(m_pRenderer);      // 화면 지우기
    SDL_RenderPresent(m_pRenderer);    // 화면 출력
}

void Game::clean() {
    if (m_pRenderer) {
        SDL_DestroyRenderer(m_pRenderer);
        m_pRenderer = nullptr;
    }
    if (m_pWindow) {
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }
    SDL_Quit();
}

bool Game::running() const {
    return m_bRunning;
}