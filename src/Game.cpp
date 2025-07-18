#include "Game.h"
#include <iostream>

// 🆕 생성자: 텍스처 포인터 초기화 추가
Game::Game() : m_bRunning(false), m_pWindow(nullptr), m_pRenderer(nullptr), m_pTexture(nullptr) {
}

Game::~Game() {
    clean();
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags) {
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

    // 배경색 설정
    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

    // 🆕 NEW: 이미지 로딩 및 텍스처 생성
    SDL_Surface* tempSurface = SDL_LoadBMP("./assets/rider.bmp");
    if (!tempSurface) {
        std::cerr << "이미지 로드 실패: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(m_pRenderer);
        SDL_DestroyWindow(m_pWindow);
        SDL_Quit();
        return false;
    }

    // Surface를 Texture로 변환
    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_FreeSurface(tempSurface); // 임시 Surface 해제

    if (!m_pTexture) {
        std::cerr << "텍스처 생성 실패: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(m_pRenderer);
        SDL_DestroyWindow(m_pWindow);
        SDL_Quit();
        return false;
    }

    m_bRunning = true;
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
    // 게임 로직 업데이트
}

void Game::render() {
    // 화면 초기화
    SDL_RenderClear(m_pRenderer);

    // 🆕 NEW: 텍스처 렌더링
    SDL_RenderCopy(m_pRenderer, m_pTexture, nullptr, nullptr);

    // 화면 출력
    SDL_RenderPresent(m_pRenderer);
}

void Game::clean() {
    // 🆕 NEW: 텍스처 해제
    if (m_pTexture) {
        SDL_DestroyTexture(m_pTexture);
        m_pTexture = nullptr;
    }

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
