#ifndef ENEMY_BEHAVIOR_H
#define ENEMY_BEHAVIOR_H

#include "MonoBehaviour.h"
#include "SpriteRenderer.h"
#include "Vector2D.h"
#include "Collider.h"

class EnemyBehavior : public MonoBehaviour {
public:

    EnemyBehavior();
    virtual ~EnemyBehavior();

    // 적 속성 설정
    void setPatrolRange(float range) { m_patrolRange = range; }
    void setMoveSpeed(float speed) { m_moveSpeed = speed; }

private:
    void Awake() override;
    void Start() override;
    void Update() override;
    void FixedUpdate() override;

    // 🆕 NEW: 충돌 이벤트 오버라이드
    void onCollisionEnter(const Collision& collision) override;
    void onCollisionStay(const Collision& collision) override;
    void onCollisionExit(const Collision& collision) override;

    // 내부 유틸리티 함수들
    void updateMovement();
    void updateAnimation();
    void flip();

    // 컴포넌트 참조
    SpriteRenderer* m_spriteRenderer = nullptr;

    // 이동 관련 변수들
    Vector2D m_startPosition;
    Vector2D m_currentDirection{ 1, 0 };
    float m_patrolRange = 200.0f;
    float m_moveSpeed = 100.0f;

    // 🆕 NEW: AI 상태 관리
    bool m_hasDetectedPlayer = false;
    float m_originalMoveSpeed;
    float m_pauseTimer = 0.0f;

    // 애니메이션 관련
    bool m_facingRight = true;
    const int WALK_FRAME_COUNT = 8;
};

#endif // ENEMY_BEHAVIOR_H