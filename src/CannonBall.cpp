#include "CannonBall.h"
#include "CollisionManager.h"
#include <cmath>

CannonBall::CannonBall(const LoaderParams* pParams)
    : SDLGameObject(pParams), m_isVisible(false), m_velocityX(0),
    m_velocityY(0), m_timeElapsed(0) {
}

void CannonBall::update(float deltaTime) {
    if (!m_isVisible) return;

    m_timeElapsed += deltaTime;

    m_position.setX(m_position.getX() + m_velocityX * deltaTime);
    m_position.setY(m_position.getY() + m_velocityY * deltaTime +
        0.5f * 9.8f * m_timeElapsed * m_timeElapsed);

    if (m_position.getX() <= 0 || m_position.getX() + m_width >= 640 ||
        m_position.getY() <= 0 || m_position.getY() + m_height >= 500) {
        m_isVisible = false;
    }
}

void CannonBall::render(SDL_Renderer* pRenderer) {
    if (!m_isVisible) return;

    SDL_Rect rect;
    rect.x = static_cast<int>(m_position.getX());
    rect.y = static_cast<int>(m_position.getY());
    rect.w = m_width;
    rect.h = m_height;

    SDL_SetRenderDrawColor(pRenderer, 255, 0, 255, 255);
    SDL_RenderFillRect(pRenderer, &rect);
}

void CannonBall::clean() {
    SDLGameObject::clean();
}

void CannonBall::onCollision(GameObject* other) {
    m_isVisible = false;
}

void CannonBall::shoot(float angle, float power) {
    m_isVisible = true;
    m_timeElapsed = 0;
    m_velocityX = power * std::cos(angle * M_PI / 180.0f);
    m_velocityY = -power * std::sin(angle * M_PI / 180.0f);
}