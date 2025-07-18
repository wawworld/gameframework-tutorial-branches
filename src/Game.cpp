#include "Game.h"
#include <iostream>

// 🔄 CHANGE: 다중 텍스처 및 입력 관련 멤버 변수 초기화 추가
Game::Game() : m_bRunning(false), m_pWindow(nullptr), m_pRenderer(nullptr),
m_pTexture(nullptr), m_pTexture2(nullptr), m_frameStart(0),
m_frameTime(0), m_frameCount(0), m_lastTime(0),
m_srcRect{ 0, 0, 0, 0 }, m_destRect{ 0, 0, 0, 0 },
m_srcRect2{ 0, 0, 0, 0 }, m_destRect2{ 0, 0, 0, 0 },
m_direction(1), m_velocityX(0), m_velocityY(0) {
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

    // 🆕 NEW: SDL_Image 초기화
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

    // 🔄 CHANGE: PNG 파일로 첫 번째 텍스처 로딩
    SDL_Surface* tempSurface = IMG_Load("./assets/animate-alpha.png");
    if (!tempSurface) {
        std::cerr << "이미지 로드 실패: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(m_pRenderer);
        SDL_DestroyWindow(m_pWindow);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!m_pTexture) {
        std::cerr << "텍스처 생성 실패: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(m_pRenderer);
        SDL_DestroyWindow(m_pWindow);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // 🆕 NEW: 두 번째 이미지 로드 (키보드 제어용)
    SDL_Surface* tempSurface2 = SDL_LoadBMP("./assets/rider.bmp");
    if (!tempSurface2) {
        std::cerr << "두 번째 이미지 로드 실패: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(m_pTexture);
        SDL_DestroyRenderer(m_pRenderer);
        SDL_DestroyWindow(m_pWindow);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    m_pTexture2 = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface2);
    SDL_FreeSurface(tempSurface2);

    if (!m_pTexture2) {
        std::cerr << "두 번째 텍스처 생성 실패: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(m_pTexture);
        SDL_DestroyRenderer(m_pRenderer);
        SDL_DestroyWindow(m_pWindow);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // 첫 번째 텍스처 설정 (애니메이션 스프라이트)
    m_srcRect = { 0, 0, 128, 82 };
    m_destRect = { 0, 200, 128, 82 };

    // 🆕 NEW: 두 번째 텍스처 설정 (키보드 제어 스프라이트)
    m_srcRect2 = { 0, 0, 128, 128 };
    m_destRect2 = { 100, 100, 128, 128 };

    SDL_SetRenderDrawColor(m_pRenderer, 100, 0, 0, 255);

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

        // 🆕 NEW: 키보드 입력 처리
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                m_velocityY = -1;
                break;  // 위로 이동
            case SDLK_DOWN:
                m_velocityY = 1;
                break;  // 아래로 이동
            case SDLK_LEFT:
                m_velocityX = -1;
                break;  // 왼쪽으로 이동
            case SDLK_RIGHT:
                m_velocityX = 1;
                break;  // 오른쪽으로 이동
            }
        }

        // 🆕 NEW: 키 떼기 처리
        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_DOWN:
                m_velocityY = 0;
                break;  // 수직 이동 멈춤
            case SDLK_LEFT:
            case SDLK_RIGHT:
                m_velocityX = 0;
                break;  // 수평 이동 멈춤
            }
        }
    }
}

void Game::update() {
    // 첫 번째 텍스처 애니메이션 (기존 코드)
    Uint32 ticks = SDL_GetTicks();
    m_srcRect.x = 128 * ((ticks / 100) % 6);

    m_destRect.x += m_direction;
    if (m_destRect.x + m_destRect.w > 640 || m_destRect.x < 0) {
        m_direction = -m_direction;
    }

    // 🆕 NEW: 두 번째 텍스처 키보드 기반 이동
    m_destRect2.x += m_velocityX;
    m_destRect2.y += m_velocityY;

    // 🆕 NEW: 두 번째 텍스처 경계 충돌 처리
    if (m_destRect2.x < 0)
        m_destRect2.x = 0;
    if (m_destRect2.x + m_destRect2.w > 640)
        m_destRect2.x = 640 - m_destRect2.w;
    if (m_destRect2.y < 0)
        m_destRect2.y = 0;
    if (m_destRect2.y + m_destRect2.h > 480)
        m_destRect2.y = 480 - m_destRect2.h;
}

void Game::render() {
    SDL_RenderClear(m_pRenderer);

    // 첫 번째 텍스처 렌더링 (애니메이션 스프라이트)
    SDL_RenderCopy(m_pRenderer, m_pTexture, &m_srcRect, &m_destRect);

    // 🆕 NEW: 두 번째 텍스처 렌더링 (키보드 제어 스프라이트)
    SDL_RenderCopy(m_pRenderer, m_pTexture2, &m_srcRect2, &m_destRect2);

    SDL_RenderPresent(m_pRenderer);
}

void Game::clean() {
    if (m_pTexture) {
        SDL_DestroyTexture(m_pTexture);
        m_pTexture = nullptr;
    }

    // 🆕 NEW: 두 번째 텍스처 정리
    if (m_pTexture2) {
        SDL_DestroyTexture(m_pTexture2);
        m_pTexture2 = nullptr;
    }

    if (m_pRenderer) {
        SDL_DestroyRenderer(m_pRenderer);
        m_pRenderer = nullptr;
    }

    if (m_pWindow) {
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = nullptr;
    }

    // 🆕 NEW: SDL_Image 종료
    IMG_Quit();
    SDL_Quit();
}

bool Game::running() const {
    return m_bRunning;
}