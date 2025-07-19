#include "Game.h"
#include <iostream>

// 🔄 CHANGE: 게임 객체 포인터 초기화
Game::Game() : m_bRunning(false), m_pWindow(nullptr), m_pRenderer(nullptr),
m_frameStart(0), m_frameTime(0), m_frameCount(0), m_lastTime(0),
m_pPlayer(nullptr), m_pEnemy(nullptr) {
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

    // 🆕 NEW: 게임 객체들 동적 생성
    m_pPlayer = new Player("animate", 100, 200, 128, 82);
    m_pEnemy = new Enemy("animate-alpha", 300, 300, 128, 82);

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
        // 🔄 CHANGE: 키보드 입력 처리는 Player 클래스에서 담당
    }
}

void Game::update() {
    // 🔄 CHANGE: 각 객체의 update() 메서드 호출
    if (m_pPlayer) {
        m_pPlayer->update();        // 플레이어 입력 처리 및 위치 업데이트
    }

    if (m_pEnemy) {
        m_pEnemy->update();         // 적 자동 이동 업데이트
    }
}

void Game::render() {
    SDL_RenderClear(m_pRenderer);

    // 🔄 CHANGE: 각 객체의 render() 메서드 호출
    if (m_pPlayer) {
        m_pPlayer->render(m_pRenderer);
    }

    if (m_pEnemy) {
        m_pEnemy->render(m_pRenderer);
    }

    SDL_RenderPresent(m_pRenderer);
}

void Game::clean() {
    // 🆕 NEW: 게임 객체 메모리 해제
    if (m_pPlayer) {
        m_pPlayer->clean();
        delete m_pPlayer;
        m_pPlayer = nullptr;
    }

    if (m_pEnemy) {
        m_pEnemy->clean();
        delete m_pEnemy;
        m_pEnemy = nullptr;
    }

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