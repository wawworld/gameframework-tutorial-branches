#include "Game.h"
#include <iostream>

// 🔄 CHANGE: 새로운 멤버 변수들 초기화 추가
Game::Game() : m_bRunning(false), m_pWindow(nullptr), m_pRenderer(nullptr),
m_pTexture(nullptr), m_frameStart(0), m_frameTime(0),
m_frameCount(0), m_lastTime(0), m_dstRect{ 0, 0, 0, 0 } {
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

    // 이미지 로딩 및 텍스처 생성
    SDL_Surface* tempSurface = SDL_LoadBMP("./assets/rider.bmp");
    if (!tempSurface) {
        std::cerr << "이미지 로드 실패: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(m_pRenderer);
        SDL_DestroyWindow(m_pWindow);
        SDL_Quit();
        return false;
    }

    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!m_pTexture) {
        std::cerr << "텍스처 생성 실패: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(m_pRenderer);
        SDL_DestroyWindow(m_pWindow);
        SDL_Quit();
        return false;
    }

    // 🆕 NEW: 텍스처 크기 및 창 크기 계산
    int texW = 0, texH = 0;
    SDL_QueryTexture(m_pTexture, nullptr, nullptr, &texW, &texH);

    int winW = 0, winH = 0;
    SDL_GetWindowSize(m_pWindow, &winW, &winH);

    // 🆕 NEW: 텍스처를 중앙에 배치
    int x = (winW - texW) / 2;
    int y = (winH - texH) / 2;
    m_dstRect = { x, y, texW, texH };

    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

    m_bRunning = true;
    return true;
}

// 🆕 NEW: 게임 루프 구현
void Game::gameLoop() {
    m_lastTime = SDL_GetTicks();

    while (running()) {
        m_frameStart = SDL_GetTicks();

        handleEvents();
        update();
        render();

        m_frameTime = SDL_GetTicks() - m_frameStart;
        m_frameCount++;

        // 1초마다 FPS 출력
        if (SDL_GetTicks() - m_lastTime >= 1000) {
            std::cout << "FPS: " << m_frameCount << std::endl;
            m_frameCount = 0;
            m_lastTime = SDL_GetTicks();
        }

        // 목표 프레임 시간보다 빨리 처리된 경우 대기
        if (m_frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - m_frameTime);
        }
    }
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
    SDL_RenderClear(m_pRenderer);

    // 🔄 CHANGE: 텍스처를 중앙에 렌더링
    SDL_RenderCopy(m_pRenderer, m_pTexture, nullptr, &m_dstRect);

    SDL_RenderPresent(m_pRenderer);
}

void Game::clean() {
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