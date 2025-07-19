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
    // 필요한 컴포넌트 얻기
    m_spriteRenderer = getComponent<SpriteRenderer>();
    if (!m_spriteRenderer)
    {
        std::cerr << "SpriteRenderer not found on Enemy" << std::endl;
        return;
    }
}

void EnemyBehavior::Start()
{
    // 시작 위치 저장
    m_startPosition = getTransform()->getPosition();
}

void EnemyBehavior::Update()
{
    updateAnimation();
}

void EnemyBehavior::FixedUpdate()
{
    updateMovement();
}

void EnemyBehavior::updateAnimation()
{
    if (!m_spriteRenderer)
        return;

    // 애니메이션은 이미 setAnimated(true)로 설정되어 자동으로 재생됨
    // 필요한 경우 여기에 추가 애니메이션 로직 구현
}

void EnemyBehavior::updateMovement()
{
    Vector2D currentPos = getTransform()->getPosition();
    float leftBound = m_startPosition.getX() - m_patrolRange;
    float rightBound = m_startPosition.getX() + m_patrolRange;

    // 경계에 도달하면 방향 전환
    if (currentPos.getX() <= leftBound || currentPos.getX() >= rightBound)
    {
        m_currentDirection.setX(-m_currentDirection.getX());
        flip();
    }

    // 시간 기반 이동 계산
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