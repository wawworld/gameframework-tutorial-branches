#include "Game.h"
#include "Transform.h"
#include "SpriteRenderer.h"     // 🆕 NEW: SpriteRenderer 컴포넌트
#include "TextureManager.h"
#include "GameTime.h"               // 🆕 NEW: Time 시스템
#include "EnemyBehavior.h"
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

    // SDL_image 초기화
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
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

    // 텍스처 로드
    loadTextures();

    // GameObject 시스템 사용
    createGameObjects();

    // 스프라이트 렌더링 테스트
    testSpriteRendering();

    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

    m_bRunning = true;
    return true;
}

void Game::loadTextures() {
    // 애니메이션 스프라이트 로드 (6열 1행)
    TextureInfo animateInfo(128, 82, 1, 6);
    TheTextureManager::Instance()->load("assets/animate.png", "animate",
        m_pRenderer, animateInfo);

    // 알파 스프라이트 로드 (6열 1행)
    TextureInfo alphaInfo(128, 82, 1, 6);
    TheTextureManager::Instance()->load("assets/animate-alpha.png", "animate-alpha",
        m_pRenderer, alphaInfo);
}

void Game::createGameObjects() {
    createPlayer();
    createEnemies();  // 🆕 NEW: 적 생성 호출

    // 모든 게임 오브젝트 초기화
    for (auto& gameObject : m_gameObjects) {
        gameObject->init();
    }
}

void Game::createPlayer() {
    auto player = std::make_shared<GameObject>("AnimatedPlayer");

    Transform* playerTransform = player->getComponent<Transform>();
    playerTransform->setPosition(Vector2D(100, 400));
    playerTransform->setScale(Vector2D(2.0f, 2.0f));

    SpriteRenderer* playerRenderer = player->addComponent<SpriteRenderer>();
    playerRenderer->setTextureID("animate");
    playerRenderer->setAnimated(true);
    playerRenderer->setAnimationSpeed(8.0f);
    playerRenderer->setColor({ 255, 255, 255, 255 });

    m_gameObjects.push_back(player);
}

void Game::createEnemies() {
    // 첫 번째 적 생성
    auto enemy1 = std::make_shared<GameObject>("Enemy1");

    Transform* enemy1Transform = enemy1->getComponent<Transform>();
    enemy1Transform->setPosition(Vector2D(300, 300));
    enemy1Transform->setScale(Vector2D(1.5f, 1.5f));

    SpriteRenderer* enemy1Renderer = enemy1->addComponent<SpriteRenderer>();
    enemy1Renderer->setTextureID("animate-alpha");
    enemy1Renderer->setAnimated(true);
    enemy1Renderer->setAnimationSpeed(4.0f);
    enemy1Renderer->setAlpha(200);

    // 🆕 NEW: EnemyBehavior 컴포넌트 추가
    EnemyBehavior* enemy1Behavior = enemy1->addComponent<EnemyBehavior>();
    enemy1Behavior->setPatrolRange(150.0f);
    enemy1Behavior->setMoveSpeed(80.0f);

    // 두 번째 적 생성
    auto enemy2 = std::make_shared<GameObject>("Enemy2");

    Transform* enemy2Transform = enemy2->getComponent<Transform>();
    enemy2Transform->setPosition(Vector2D(500, 200));
    enemy2Transform->setScale(Vector2D(1.2f, 1.2f));

    SpriteRenderer* enemy2Renderer = enemy2->addComponent<SpriteRenderer>();
    enemy2Renderer->setTextureID("animate");
    enemy2Renderer->setAnimated(true);
    enemy2Renderer->setAnimationSpeed(6.0f);
    enemy2Renderer->setColor({ 255, 200, 200, 255 });

    EnemyBehavior* enemy2Behavior = enemy2->addComponent<EnemyBehavior>();
    enemy2Behavior->setPatrolRange(250.0f);
    enemy2Behavior->setMoveSpeed(120.0f);

    // 게임 오브젝트 등록
    m_gameObjects.push_back(enemy1);
    m_gameObjects.push_back(enemy2);
}

void Game::testSpriteRendering() {
    std::cout << "=== 컴포넌트 기반 행동 제어 시스템 테스트 ===" << std::endl;

    for (auto& gameObject : m_gameObjects) {
        std::cout << "GameObject: " << gameObject->getName() << std::endl;

        Transform* transform = gameObject->getComponent<Transform>();
        if (transform) {
            Vector2D pos = transform->getPosition();
            Vector2D scale = transform->getScale();
            float rotation = transform->getRotation();

            std::cout << "  Transform - 위치: (" << pos.getX() << ", " << pos.getY()
                << "), 크기: (" << scale.getX() << ", " << scale.getY()
                << "), 회전: " << rotation << "도" << std::endl;
        }

        SpriteRenderer* renderer = gameObject->getComponent<SpriteRenderer>();
        if (renderer) {
            std::cout << "  SpriteRenderer - 컴포넌트 활성화됨" << std::endl;
        }

        // 🆕 NEW: EnemyBehavior 컴포넌트 확인
        EnemyBehavior* behavior = gameObject->getComponent<EnemyBehavior>();
        if (behavior) {
            std::cout << "  EnemyBehavior - 적 AI 컴포넌트 활성화됨" << std::endl;
        }
    }
    std::cout << "================================================" << std::endl;
}

void Game::gameLoop() {
    using namespace std::chrono;

    auto lastTime = steady_clock::now();
    auto lastFPSTime = lastTime;

    while (running()) {
        auto currentTime = steady_clock::now();

        // Time 시스템 업데이트
        TheTime::Instance()->update();

        handleEvents();
        update();
        render();

        m_frameCount++;

        if (duration_cast<seconds>(currentTime - lastFPSTime).count() >= 1) {
            std::cout << "FPS: " << TheTime::Instance()->getFPS()
                << " | GameObjects: " << m_gameObjects.size()
                << " | DeltaTime: " << TheTime::Instance()->getDeltaTime() << "s" << std::endl;
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

        // 키보드 테스트 (스페이스바로 적 행동 일시정지)
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            for (auto& gameObject : m_gameObjects) {
                EnemyBehavior* behavior = gameObject->getComponent<EnemyBehavior>();
                if (behavior) {
                    behavior->setActive(!behavior->isActive());
                }
            }
        }
    }
}

void Game::update() {
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