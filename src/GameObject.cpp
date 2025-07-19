#include "GameObject.h"

GameObject::GameObject(const LoaderParams* pParams)
    : m_textureID(pParams->getTextureID()),
    m_x(pParams->getX()),
    m_y(pParams->getY()),
    m_width(pParams->getWidth()),
    m_height(pParams->getHeight()),
    m_velocityX(0),
    m_velocityY(0),
    m_currentFrame(0),
    m_currentRow(1) {
}

void GameObject::setVelocity(int x, int y) {
    m_velocityX = x;
    m_velocityY = y;
}

void GameObject::setPosition(int x, int y) {
    m_x = x;
    m_y = y;
}