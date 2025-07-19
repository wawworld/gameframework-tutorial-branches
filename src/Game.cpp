#include "Game.h"
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

    // 윈도우 생성
    m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (m_pWindow == nullptr) {
        std::cerr << "윈도우 생성 실패: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // 렌더러 생성
    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_pRenderer == nullptr) {
        std::cerr << "렌더러 생성 실패: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_pWindow);
        SDL_Quit();
        return false;
    }

    // GameObject 시스템 사용
    createGameObjects();

    // 컴포넌트 시스템 테스트
    testComponentSystem();

    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

    m_bRunning = true;
    return true;
}

void Game::createGameObjects() {
    // 플레이어 게임 오브젝트 생성
    auto player = std::make_shared<GameObject>("Player");

    // Transform 컴포넌트를 통한 위치 설정
    Transform* playerTransform = player->getComponent<Transform>();
    playerTransform->setPosition(Vector2D(100, 400));
    playerTransform->setScale(Vector2D(2.0f, 2.0f));

    // 적 게임 오브젝트 생성
    auto enemy = std::make_shared<GameObject>("Enemy");

    Transform* enemyTransform = enemy->getComponent<Transform>();
    enemyTransform->setPosition(Vector2D(300, 300));
    enemyTransform->setRotation(45.0f);

    // 게임 오브젝트 컨테이너에 추가
    m_gameObjects.push_back(player);
    m_gameObjects.push_back(enemy);

    // 모든 게임 오브젝트 초기화
    for (auto& gameObject : m_gameObjects) {
        gameObject->init();
    }
}

void Game::testComponentSystem() {
    std::cout << "=== 컴포넌트 시스템 테스트 ===" << std::endl;

    for (auto& gameObject : m_gameObjects) {
        Transform* transform = gameObject->getComponent<Transform>();
        if (transform) {
            Vector2D pos = transform->getPosition();
            Vector2D scale = transform->getScale();
            float rotation = transform->getRotation();

            std::cout << gameObject->getName() << " - "
                << "위치: (" << pos.getX() << ", " << pos.getY() << "), "
                << "크기: (" << scale.getX() << ", " << scale.getY() << "), "
                << "회전: " << rotation << "도" << std::endl;

            // 테스트: 위치 이동
            transform->translate(Vector2D(10, 5));
            transform->rotate(15.0f);

            Vector2D newPos = transform->getPosition();
            float newRotation = transform->getRotation();
            std::cout << "  → 이동 후 위치: (" << newPos.getX() << ", " << newPos.getY()
                << "), 회전: " << newRotation << "도" << std::endl;
        }
    }
    std::cout << "==============================" << std::endl;
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
            std::cout << "FPS: " << m_frameCount << " | GameObjects: " << m_gameObjects.size() << std::endl;
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
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_bRunning = false;
        }
    }
}

void Game::update(float deltaTime) {
    for (auto& gameObject : m_gameObjects) {
        gameObject->update();
        gameObject->fixedUpdate();
    }
}

void Game::render() {
    SDL_RenderClear(m_pRenderer);

    for (auto& gameObject : m_gameObjects) {
        gameObject->render(m_pRenderer);
    }

    SDL_RenderPresent(m_pRenderer);
}

void Game::clean() {
    for (auto& gameObject : m_gameObjects) {
        gameObject->destroy();
    }
    m_gameObjects.clear();

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