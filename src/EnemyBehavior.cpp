#include "EnemyBehavior.h"
#include "GameTime.h"
#include <iostream>


EnemyBehavior::EnemyBehavior()
    : m_spriteRenderer(nullptr)
    , m_startPosition(0, 0)
    , m_currentDirection(1, 0)
    , m_patrolRange(200.0f)
    , m_moveSpeed(100.0f)
    , m_facingRight(true)
{
    // 생성자에서 멤버 변수들 명시적 초기화
}

EnemyBehavior::~EnemyBehavior()
{
    // 안전한 소멸자 - 포인터 참조 정리
    m_spriteRenderer = nullptr;
}
void EnemyBehavior::Awake()
{
    m_spriteRenderer = getComponent<SpriteRenderer>();
    if (!m_spriteRenderer) {
        std::cerr << "SpriteRenderer not found on Enemy" << std::endl;
        return;
    }
}

void EnemyBehavior::Start()
{
    m_startPosition = getTransform()->getPosition();
    m_originalMoveSpeed = m_moveSpeed; // 🆕 NEW: 원래 속도 저장
}

void EnemyBehavior::Update()
{
    updateAnimation();
}

void EnemyBehavior::FixedUpdate()
{
    updateMovement();
}

// 🆕 NEW: 충돌 이벤트 구현
void EnemyBehavior::onCollisionEnter(const Collision& collision)
{
    Collider* otherCollider = collision.other->getComponent<Collider>();
    if (otherCollider && otherCollider->getLayer() == CollisionLayer::Player)
    {
        std::cout << "Enemy detected player entry!" << std::endl;

        m_hasDetectedPlayer = true;
        m_moveSpeed *= 0.5f; // 속도 감소

        // 플레이어 방향으로 회전
        Vector2D playerPos = collision.other->getTransform()->getPosition();
        Vector2D myPos = getTransform()->getPosition();
        Vector2D direction = (playerPos - myPos);
        direction.normalize();
        m_currentDirection = direction;
    }
}

void EnemyBehavior::onCollisionStay(const Collision& collision)
{
    Collider* otherCollider = collision.other->getComponent<Collider>();
    if (otherCollider && otherCollider->getLayer() == CollisionLayer::Player)
    {
        std::cout << "Enemy engaging player!" << std::endl;

        // 지속적인 플레이어 추적
        Vector2D playerPos = collision.other->getTransform()->getPosition();
        Vector2D myPos = getTransform()->getPosition();
        Vector2D direction = (playerPos - myPos);
        direction.normalize();

        // 플레이어를 향해 천천히 이동
        Vector2D velocity = direction * (m_moveSpeed * 0.3f);
        Vector2D newPos = myPos + (velocity * TheTime::getDeltaTime());
        getTransform()->setPosition(newPos);
    }
}

void EnemyBehavior::onCollisionExit(const Collision& collision)
{
    Collider* otherCollider = collision.other->getComponent<Collider>();
    if (otherCollider && otherCollider->getLayer() == CollisionLayer::Player)
    {
        std::cout << "Enemy lost sight of player!" << std::endl;

        // 플레이어를 놓쳤을 때 원래 행동으로 복귀
        m_hasDetectedPlayer = false;
        m_moveSpeed = m_originalMoveSpeed; // 원래 속도 복구

        // 잠시 멈춘 후 순찰 재개
        m_pauseTimer = 2.0f; // 2초간 일시정지
    }
}

void EnemyBehavior::updateAnimation()
{
    if (!m_spriteRenderer)
        return;

    // 애니메이션은 이미 setAnimated(true)로 설정되어 자동으로 재생됨
}

void EnemyBehavior::updateMovement()
{
    // 일시정지 처리
    if (m_pauseTimer > 0.0f)
    {
        m_pauseTimer -= TheTime::getDeltaTime();
        return;
    }

    // 플레이어 감지 상태에 따른 분기
    if (m_hasDetectedPlayer)
    {
        // 플레이어가 감지된 상태에서는 충돌 처리에서 이동 담당
        return;
    }

    // 기존 순찰 로직
    Vector2D currentPos = getTransform()->getPosition();
    float leftBound = m_startPosition.getX() - m_patrolRange;
    float rightBound = m_startPosition.getX() + m_patrolRange;

    if (currentPos.getX() <= leftBound || currentPos.getX() >= rightBound)
    {
        m_currentDirection.setX(-m_currentDirection.getX());
        flip();
    }

    Vector2D velocity = m_currentDirection * m_moveSpeed;
    Vector2D newPos = currentPos + (velocity * TheTime::getDeltaTime());
    getTransform()->setPosition(newPos);
}

void EnemyBehavior::flip()
{
    m_facingRight = !m_facingRight;
    if (m_spriteRenderer)
    {
        SDL_RendererFlip flip = m_facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
        m_spriteRenderer->setFlip(flip);
    }
}