#include "Enemy.h"
#include "InputHandler.h"
#include <iostream>

Enemy::Enemy(const LoaderParams* pParams)
    : SDLGameObject(pParams) {
    m_acceleration.setX(50);
}

void Enemy::update(float deltaTime) {
    handleInput();

    if (m_position.getX() + m_width > 640 || m_position.getX() < 0) {
        m_acceleration.setX(-m_acceleration.getX());
        m_velocity.setX(0);
    }

    SDLGameObject::update(deltaTime);
}

void Enemy::handleInput() {
    if (InputHandler::Instance()->getMouseButtonState(LEFT)) {
        std::cout << "Left Button Clicked" << std::endl;
    }
}

// 🆕 NEW: 적 충돌 처리 구현
void Enemy::onCollision(GameObject* other) {
    std::cout << "Enemy collided with object!" << std::endl;

    // 충돌 시 방향 전환
    m_acceleration.setX(-m_acceleration.getX());
    m_velocity.setX(0);
}