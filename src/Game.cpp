#include "Game.h"
#include "Transform.h"
#include "SpriteRenderer.h"     // 🆕 NEW: SpriteRenderer 컴포넌트
#include "TextureManager.h"
#include "GameTime.h"               // 🆕 NEW: Time 시스템

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

    // 🆕 NEW: SDL_image 초기화
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

    // 🆕 NEW: 텍스처 로드
    loadTextures();

    // GameObject 시스템 사용
    createGameObjects();

    // 🆕 NEW: 스프라이트 렌더링 테스트
    testSpriteRendering();

    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);

    m_bRunning = true;
    return true;
}

// 🆕 NEW: 텍스처 로딩 메서드
void Game::loadTextures() {
    // 애니메이션 스프라이트 로드 (6열 1행)
    TextureInfo animateInfo(128, 82, 1, 6);  // 프레임크기, 행, 열
    TheTextureManager::Instance()->load("assets/animate.png", "animate",
        m_pRenderer, animateInfo);

    // 알파 스프라이트 로드 (6열 1행)
    TextureInfo alphaInfo(128, 82, 1, 6);
    TheTextureManager::Instance()->load("assets/animate-alpha.png", "animate-alpha",
        m_pRenderer, alphaInfo);
}

// 🔄 CHANGE: SpriteRenderer 컴포넌트 활용
void Game::createGameObjects() {
    // 🆕 NEW: 애니메이션 플레이어 생성
    auto player = std::make_shared<GameObject>("AnimatedPlayer");

    // Transform 설정
    Transform* playerTransform = player->getComponent<Transform>();
    playerTransform->setPosition(Vector2D(100, 400));
    playerTransform->setScale(Vector2D(2.0f, 2.0f));

    // 🆕 NEW: SpriteRenderer 컴포넌트 추가
    SpriteRenderer* playerRenderer = player->addComponent<SpriteRenderer>();
    playerRenderer->setTextureID("animate");
    playerRenderer->setAnimated(true);
    playerRenderer->setAnimationSpeed(8.0f);  // 8프레임/초
    playerRenderer->setColor({ 255, 255, 255, 255 });

    // 🆕 NEW: 정적 적 오브젝트 생성
    auto enemy = std::make_shared<GameObject>("StaticEnemy");

    Transform* enemyTransform = enemy->getComponent<Transform>();
    enemyTransform->setPosition(Vector2D(300, 300));
    enemyTransform->setRotation(45.0f);

    // 🆕 NEW: 정적 스프라이트 렌더러
    SpriteRenderer* enemyRenderer = enemy->addComponent<SpriteRenderer>();
    enemyRenderer->setTextureID("animate-alpha");
    enemyRenderer->setAnimated(false);
    enemyRenderer->setCurrentFrame(0, 2);  // 특정 프레임 고정
    enemyRenderer->setAlpha(128);          // 50% 투명도

    // 🆕 NEW: 회전하는 스프라이트 생성
    auto rotatingSprite = std::make_shared<GameObject>("RotatingSprite");

    Transform* rotatingTransform = rotatingSprite->getComponent<Transform>();
    rotatingTransform->setPosition(Vector2D(500, 200));
    rotatingTransform->setScale(Vector2D(1.5f, 1.5f));

    SpriteRenderer* rotatingRenderer = rotatingSprite->addComponent<SpriteRenderer>();
    rotatingRenderer->setTextureID("animate");
    rotatingRenderer->setAnimated(true);
    rotatingRenderer->setAnimationSpeed(4.0f);
    rotatingRenderer->setFlip(SDL_FLIP_HORIZONTAL);

    // 게임 오브젝트 등록
    m_gameObjects.push_back(player);
    m_gameObjects.push_back(enemy);
    m_gameObjects.push_back(rotatingSprite);

    // 모든 게임 오브젝트 초기화
    for (auto& gameObject : m_gameObjects) {
        gameObject->init();
    }
}

// 🆕 NEW: 스프라이트 렌더링 시스템 테스트
void Game::testSpriteRendering() {
    std::cout << "=== 스프라이트 렌더링 시스템 테스트 ===" << std::endl;

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
    }
    std::cout << "=========================================" << std::endl;
}

// 🔄 CHANGE: Time 시스템 통합
void Game::gameLoop() {
    using namespace std::chrono;

    auto lastTime = steady_clock::now();
    auto lastFPSTime = lastTime;

    while (running()) {
        auto currentTime = steady_clock::now();

        // 🆕 NEW: Time 시스템 업데이트
        TheTime::Instance()->update();

        handleEvents();
        update();  // deltaTime 매개변수 제거
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

// 🔄 CHANGE: 키보드 이벤트 추가
void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_bRunning = false;
        }

        // 🆕 NEW: 키보드 테스트 (스페이스바로 회전 테스트)
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
            if (!m_gameObjects.empty()) {
                Transform* transform = m_gameObjects[1]->getComponent<Transform>();
                if (transform) {
                    transform->rotate(45.0f);
                }
            }
        }
    }
}

// 🔄 CHANGE: 지속적인 회전 애니메이션 추가
void Game::update() {
    // 🆕 NEW: 지속적인 회전 애니메이션 (세 번째 오브젝트)
    if (m_gameObjects.size() > 2) {
        Transform* rotatingTransform = m_gameObjects[2]->getComponent<Transform>();
        if (rotatingTransform) {
            rotatingTransform->rotate(30.0f * TheTime::Instance()->getDeltaTime());
        }
    }

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

// 🔄 CHANGE: SDL_image 정리 추가
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

    IMG_Quit();  // 🆕 NEW: SDL_image 정리
    SDL_Quit();
}

bool Game::running() const {
    return m_bRunning;
}