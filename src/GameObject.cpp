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
    // 🆕 NEW: 시간 기반 물리 업데이트
    m_velocity += m_acceleration * deltaTime;
    m_position += m_velocity * deltaTime;
}

void GameObject::setVelocity(int x, int y) {
    m_velocity.setX(x);
    m_velocity.setY(y);
}

void GameObject::setPosition(int x, int y) {
    m_position.setX(x);
    m_position.setY(y);
}