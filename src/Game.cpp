#include "Game.h"
#include "Cannon.h"        // 🆕 NEW: Cannon 클래스 헤더 추가
#include "Target.h"        // 🆕 NEW: Target 클래스 헤더 추가
#include "CollisionManager.h"
#include "InputHandler.h"
#include <iostream>

Game* Game::s_pInstance = nullptr;

Game::Game() : m_bRunning(false), m_pWindow(nullptr), m_pRenderer(nullptr),
m_frameCount(0) {
}

Game::~Game() {
    clean();
}

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL 초기화 실패: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image 초기화 실패: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (m_pWindow == nullptr) {
        std::cerr << "윈도우 생성 실패: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_pRenderer == nullptr) {
        std::cerr << "렌더러 생성 실패: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_pWindow);
        IMG_Quit();
        SDL_Quit();
        return false;
    }

    // Cannon 객체 생성
    Cannon* cannon = new Cannon(new LoaderParams(100, 460, 50, 20, "Cannon"));
    m_gameObjects.push_back(cannon);

    // Target 객체 생성
    for (int i = 0; i < 5; ++i) {
        Target* target = new Target(new LoaderParams(50 + i * 100, 50, 50, 50, "Target"));
        m_gameObjects.push_back(target);
        CollisionManager::Instance()->addGameObject(target);
    }

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

void Game::update(float deltaTime) {
    for (auto& gameObject : m_gameObjects) {
        gameObject->update(deltaTime);
    }

    CollisionManager::Instance()->update();
}

void Game::render() {
    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
    SDL_RenderClear(m_pRenderer);

    for (auto& gameObject : m_gameObjects) {
        gameObject->render(m_pRenderer);
    }

    SDL_RenderPresent(m_pRenderer);
}

void Game::clean() {
    CollisionManager::Instance()->clearGameObjects();

    for (auto& gameObject : m_gameObjects) {
        delete gameObject;
    }
    m_gameObjects.clear();

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