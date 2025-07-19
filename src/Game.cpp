#include "Game.h"
#include <iostream>

// 🆕 NEW: 정적 포인터 초기화 (클래스 외부에서 정의)
Game* Game::s_pInstance = nullptr;

Game::Game() : m_bRunning(false), m_pWindow(nullptr), m_pRenderer(nullptr),
m_frameStart(0), m_frameTime(0), m_frameCount(0), m_lastTime(0) {
    // 싱글톤 인스턴스 생성 시 초기화
}

Game::~Game() {
    clean();
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags) {
    // SDL 초기화
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL 초기화 실패: " << SDL_GetError() << std::endl;
        return false;
    }

    // SDL_Image 초기화
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image 초기화 실패: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // 윈도우 생성
    m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (m_pWindow == nullptr) {
        std::cerr << "윈도우 생성 실패: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // 렌더러 생성
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_pRenderer == nullptr) {
        std::cerr << "렌더러 생성 실패: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_pWindow);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // TextureManager를 사용한 텍스처 로딩
    if (!TheTextureManager::Instance()->load("./assets/animate.png", "animate", m_pRenderer)) {
        std::cerr << "텍스처 로드 실패: animate.png" << std::endl;
        return false;
    }

    if (!TheTextureManager::Instance()->load("./assets/animate-alpha.png", "animate-alpha", m_pRenderer)) {
        std::cerr << "텍스처 로드 실패: animate-alpha.png" << std::endl;
        return false;
    }

    // 게임 객체 생성 및 벡터에 추가
    m_gameObjects.push_back(new Player("animate", 100, 200, 128, 82));
    m_gameObjects.push_back(new Enemy("animate-alpha", 300, 300, 128, 82));

    SDL_SetRenderDrawColor(m_pRenderer, 100, 0, 100, 255);

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

void Game::update() {
    // 범위 기반 for문으로 모든 객체 업데이트
    for (auto& gameObject : m_gameObjects) {
        gameObject->update();
    }
}

void Game::render() {
    SDL_RenderClear(m_pRenderer);

    // 모든 게임 객체를 동일한 방식으로 렌더링
    for (auto& gameObject : m_gameObjects) {
        gameObject->render(m_pRenderer);
    }

    SDL_RenderPresent(m_pRenderer);
}

void Game::clean() {
    // 벡터의 모든 객체 메모리 해제
    for (auto& gameObject : m_gameObjects) {
        delete gameObject;
    }
    m_gameObjects.clear();

    // TextureManager 정리 및 SDL 종료
    TheTextureManager::Instance()->clearFromTextureMap("animate");
    TheTextureManager::Instance()->clearFromTextureMap("animate-alpha");

    if (m_pRenderer) {
        SDL_DestroyRenderer(m_pRenderer);
        m_pRenderer = nullptr;
    }

    if (m_pWindow) {
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }

    IMG_Quit();
    SDL_Quit();
}

bool Game::running() const {
    return m_bRunning;
}