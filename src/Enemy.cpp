#include "Enemy.h"
#include "InputHandler.h"
#include <iostream>

Enemy::Enemy(const LoaderParams* pParams)
    : SDLGameObject(pParams) {
    // 🆕 NEW: 초기 가속도 설정
    m_acceleration.setX(50);
}

void Enemy::update(float deltaTime) {
    // 🆕 NEW: 마우스 입력 처리
    handleInput();

    // 🆕 NEW: 마우스 위치로 이동
    int mouseX = InputHandler::Instance()->getMousePosition()->getX();
    int mouseY = InputHandler::Instance()->getMousePosition()->getY();

    // 적의 중심 좌표 계산
    float targetX = static_cast<float>(mouseX) - m_width / 2.0f;
    float targetY = static_cast<float>(mouseY) - m_height / 2.0f;

    // 부드럽게 이동 (선형 보간)
    // 목표 위치까지의 방향 벡터 계산
    float lerpFactor = 1.9f; // 이동 속도 조절
    float dirX = targetX - m_position.getX();
    float dirY = targetY - m_position.getY();

    // 선형 보간 계수 적용하여 속도 설정
    m_velocity.setX(dirX * lerpFactor);
    m_velocity.setY(dirY * lerpFactor);

    SDLGameObject::update(deltaTime);
}


void Enemy::handleInput() {
    if (InputHandler::Instance()->getMouseButtonState(LEFT)) {
        std::cout << "Left Button Clicked" << std::endl;
    }
}