#include "Enemy.h"

Enemy::Enemy(const LoaderParams* pParams)
    : SDLGameObject(pParams) {
    // 🆕 NEW: 초기 가속도 설정
    m_acceleration.setX(50);
}

void Enemy::update(float deltaTime) {
    // 🆕 NEW: 벡터 기반 경계 충돌 검사
    if (m_position.getX() + m_width > 640 || m_position.getX() < 0) {
        m_acceleration.setX(-m_acceleration.getX());
        m_velocity.setX(0);
    }

    SDLGameObject::update(deltaTime);
}