#include "Game.h"
#include <iostream>

// 🔄 CHANGE: 점프 및 애니메이션 관련 초기화 추가
Game::Game() : m_bRunning(false), m_pWindow(nullptr), m_pRenderer(nullptr),
m_frameStart(0), m_frameTime(0), m_frameCount(0), m_lastTime(0),
m_srcRect{ 0, 0, 0, 0 }, m_destRect{ 0, 0, 0, 0 },
m_srcRect2{ 0, 0, 0, 0 }, m_destRect2{ 0, 0, 0, 0 },
m_direction(1), m_velocityX(0), m_velocityY(0),
m_isJumping(false), m_jumpStartY(200), m_jumpHeight(100),
m_currentFrame(0), m_lastFrameTime(0) {
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

    // 🔄 CHANGE: TextureManager를 사용한 텍스처 로딩
    if (!TheTextureManager::Instance()->load("./assets/animate.png", "animate", m_pRenderer)) {
        std::cerr << "텍스처 로드 실패: animate.png" << std::endl;
        return false;
    }

    if (!TheTextureManager::Instance()->load("./assets/animate-alpha.png", "animate-alpha", m_pRenderer)) {
        std::cerr << "텍스처 로드 실패: animate-alpha.png" << std::endl;
        return false;
    }

    // 🔄 CHANGE: 개별 텍스처 변수 대신 destRect만 설정
    m_destRect = { 100, 200, 128, 82 };        // 플레이어 위치
    m_destRect2 = { 300, 300, 128, 82 };       // 두 번째 오브젝트 위치
    m_jumpStartY = m_destRect.y;               // 점프 기준점 설정

    SDL_SetRenderDrawColor(m_pRenderer, 0, 100, 0, 255);

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

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                m_velocityX = -1;
                break;
            case SDLK_RIGHT:
                m_velocityX = 1;
                break;
                // 🆕 NEW: 스페이스바로 점프 시작
            case SDLK_SPACE:
                if (!m_isJumping) {
                    m_isJumping = true;
                    m_jumpStartY = m_destRect.y;  // 현재 위치를 기준점으로 설정
                }
                break;
            }
        }

        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_RIGHT:
                m_velocityX = 0;
                break;
            }
        }
    }
}

void Game::update() {
    Uint32 currentTime = SDL_GetTicks();

    // 🔄 CHANGE: 애니메이션 프레임 업데이트 (시간 기반)
    if (currentTime - m_lastFrameTime > 100) {  // 100ms마다 프레임 변경
        m_currentFrame = (m_currentFrame + 1) % 6;  // 0~5 프레임 순환
        m_lastFrameTime = currentTime;
    }

    // 기존 좌우 이동 처리
    m_destRect.x += m_velocityX;
    if (m_destRect.x < 0) m_destRect.x = 0;
    if (m_destRect.x + m_destRect.w > 640) m_destRect.x = 640 - m_destRect.w;

    // 🆕 NEW: 점프 물리 처리
    if (m_isJumping) {
        m_destRect.y -= 5;  // 위로 이동 (점프 상승)
        // 최대 높이에 도달하면 하강 시작
        if (m_destRect.y <= m_jumpStartY - m_jumpHeight) {
            m_isJumping = false;
        }
    }
    else if (m_destRect.y < m_jumpStartY) {
        m_destRect.y += 5;  // 하강 처리
        // 원래 위치에 도달하면 착지
        if (m_destRect.y >= m_jumpStartY) {
            m_destRect.y = m_jumpStartY;
        }
    }

    // 두 번째 오브젝트 자동 이동 (기존 로직)
    m_destRect2.x += m_direction;
    if (m_destRect2.x + m_destRect2.w > 640 || m_destRect2.x < 0) {
        m_direction = -m_direction;
    }
}

void Game::render() {
    SDL_RenderClear(m_pRenderer);

    // 🔄 CHANGE: TextureManager를 사용한 애니메이션 렌더링
    TheTextureManager::Instance()->drawFrame("animate",
        m_destRect.x, m_destRect.y,
        128, 82,
        1, m_currentFrame,
        m_pRenderer,
        (m_velocityX < 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

    // 🔄 CHANGE: TextureManager를 사용한 일반 텍스처 렌더링
    TheTextureManager::Instance()->draw("animate-alpha",
        m_destRect2.x, m_destRect2.y,
        128, 82,
        m_pRenderer);

    SDL_RenderPresent(m_pRenderer);
}

void Game::clean() {
    // 🔄 CHANGE: TextureManager를 사용한 텍스처 정리
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