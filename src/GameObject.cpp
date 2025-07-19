#include "GameObject.h"

GameObject::GameObject(const LoaderParams* pParams)
    : m_textureID(pParams->getTextureID()),
    m_position(pParams->getX(), pParams->getY()),
    m_width(pParams->getWidth()),
    m_height(pParams->getHeight()),
    m_velocity(0, 0),
    m_acceleration(0, 0),
    m_currentFrame(0),
    m_currentRow(1) {
}

void GameObject::update(float deltaTime) {
    m_velocity += m_acceleration * deltaTime;
    m_position += m_velocity * deltaTime;
}

// 🆕 NEW: 충돌 처리 기본 구현
void GameObject::onCollision(GameObject* other) {
    // 기본적으로 아무것도 하지 않음
}

void GameObject::setVelocity(int x, int y) {
    m_velocity.setX(x);
    m_velocity.setY(y);
}

void GameObject::setPosition(int x, int y) {
    m_position.setX(x);
    m_position.setY(y);
}