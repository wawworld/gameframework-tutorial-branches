#ifndef ENEMY_BEHAVIOR_H
#define ENEMY_BEHAVIOR_H

#include "MonoBehaviour.h"
#include "SpriteRenderer.h"
#include "Vector2D.h"

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

    // 내부 유틸리티 함수들
    void updateMovement();
    void updateAnimation();
    void flip();

    // 컴포넌트 참조
    SpriteRenderer* m_spriteRenderer = nullptr;

    // 이동 관련 변수들
    Vector2D m_startPosition;
    Vector2D m_currentDirection{ 1, 0 };  // 초기 방향은 오른쪽
    float m_patrolRange = 200.0f;       // 이동 범위
    float m_moveSpeed = 100.0f;         // 이동 속도

    // 애니메이션 관련
    bool m_facingRight = true;
    const int WALK_FRAME_COUNT = 8;     // 걷기 애니메이션 프레임 수
};

#endif // ENEMY_BEHAVIOR_H