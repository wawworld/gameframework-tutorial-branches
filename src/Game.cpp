#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "LoaderParams.h"
#include "CollisionManager.h"  // 🆕 NEW: 충돌 관리자 헤더 추가
#include <iostream>

Game* Game::s_pInstance = nullptr;

Game::Game() : m_bRunning(false), m_pWindow(nullptr), m_pRenderer(nullptr),
m_frameCount(0) {
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

    // 🔄 CHANGE: 객체 생성 시 CollisionManager에 등록
    Player* player = new Player(new LoaderParams(100, 200, 128, 82, "animate"));
    m_gameObjects.push_back(player);
    CollisionManager::Instance()->addGameObject(player); // 🆕 NEW: 충돌 관리자에 등록

    Enemy* enemy = new Enemy(new LoaderParams(300, 300, 128, 82, "animate-alpha"));
    m_gameObjects.push_back(enemy);
    CollisionManager::Instance()->addGameObject(enemy); // 🆕 NEW: 충돌 관리자에 등록

    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

    m_bRunning = true;
    return true;
}

void Game::gameLoop() {
    using namespace std::chrono;

    auto lastTime = steady_clock::now();
    auto lastFPSTime = lastTime;

    while (running()) {
        auto currentTime = steady_clock::now();

        float deltaTime = duration_cast<duration<float>>(currentTime - lastTime).count();
        lastTime = currentTime;

        handleEvents();
        update(deltaTime);
        render();

        m_frameCount++;

        if (duration_cast<seconds>(currentTime - lastFPSTime).count() >= 1) {
            std::cout << "FPS: " << m_frameCount << ":: size " << m_gameObjects.size() << std::endl;
            m_frameCount = 0;
            lastFPSTime = currentTime;
        }

        auto frameTime = duration_cast<milliseconds>(steady_clock::now() - currentTime).count();

        if (frameTime < FRAME_DELAY) {
            std::this_thread::sleep_for(milliseconds(FRAME_DELAY - frameTime));
        }
    }
}

void Game::handleEvents() {
    InputHandler::Instance()->update();
}

// 🔄 CHANGE: 충돌 검사 루프 추가
void Game::update(float deltaTime) {
    for (auto& gameObject : m_gameObjects) {
        gameObject->update(deltaTime);
    }

    // 🆕 NEW: 충돌 검사 업데이트
    CollisionManager::Instance()->update();
}

void Game::render() {
    SDL_RenderClear(m_pRenderer);

    for (auto& gameObject : m_gameObjects) {
        gameObject->render(m_pRenderer);
    }

    SDL_RenderPresent(m_pRenderer);
}

// 🔄 CHANGE: 게임 정리 시 CollisionManager 정리
void Game::clean() {
    // 🆕 NEW: CollisionManager 정리
    CollisionManager::Instance()->clearGameObjects();

    for (auto& gameObject : m_gameObjects) {
        delete gameObject;
    }
    m_gameObjects.clear();

    TheTextureManager::Instance()->clearFromTextureMap("animate");
    TheTextureManager::Instance()->clearFromTextureMap("animate-alpha");

    if (InputHandler::Instance() != nullptr) {
        InputHandler::Instance()->clean();
        delete InputHandler::Instance();
    }

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