#include "Game.h"
#include <iostream>

// 🔄 CHANGE: 애니메이션 관련 멤버 변수 초기화 추가
Game::Game() : m_bRunning(false), m_pWindow(nullptr), m_pRenderer(nullptr),
m_pTexture(nullptr), m_frameStart(0), m_frameTime(0),
m_frameCount(0), m_lastTime(0), m_srcRect{ 0, 0, 0, 0 },
m_destRect{ 0, 0, 0, 0 }, m_direction(1) {
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

    // 🔄 CHANGE: 애니메이션 스프라이트 시트 로딩
    SDL_Surface* tempSurface = SDL_LoadBMP("./assets/animate.bmp");
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

    // 🆕 NEW: 원본 상자 설정 (첫 번째 프레임)
    m_srcRect = { 0, 0, 128, 82 };  // 각 프레임 크기 128x82

    // 🆕 NEW: 대상 상자 설정 (화면 시작 위치)
    m_destRect = { 0, 200, 128, 82 };  // 화면 왼쪽에서 시작, y=200 위치

    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

    m_bRunning = true;
    return true;
}

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

// 🔄 CHANGE: 애니메이션 및 이동 처리 추가
void Game::update() {
    // 🆕 NEW: 프레임 애니메이션 처리
    Uint32 ticks = SDL_GetTicks();
    m_srcRect.x = 128 * ((ticks / 100) % 6);  // 0.1초마다 프레임 전환

    // 🆕 NEW: 좌우 이동 처리
    m_destRect.x += m_direction;

    // 🆕 NEW: 경계 충돌 및 방향 전환
    if (m_destRect.x + m_destRect.w > 640 || m_destRect.x < 0) {
        m_direction = -m_direction;
    }
}

void Game::render() {
    SDL_RenderClear(m_pRenderer);

    // 🔄 CHANGE: 원본 상자와 대상 상자를 모두 지정하여 렌더링
    SDL_RenderCopy(m_pRenderer, m_pTexture, &m_srcRect, &m_destRect);

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