#include "Cannon.h"
#include "CannonBall.h"
#include "CollisionManager.h"
#include "InputHandler.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

Cannon::Cannon(const LoaderParams* pParams)
    : SDLGameObject(pParams), m_angle(45.0f), m_power(50.0f) {

    m_cannonBall = new CannonBall(new LoaderParams(m_position.getX(), m_position.getY(),
        10, 10, "CannonBall"));
    CollisionManager::Instance()->addGameObject(m_cannonBall);
}

Cannon::~Cannon() {
    delete m_cannonBall;
}

void Cannon::update(float deltaTime) {
    handleInput();
    m_cannonBall->update(deltaTime);
}

void Cannon::render(SDL_Renderer* pRenderer) {
    SDL_Rect rect;
    rect.x = static_cast<int>(m_position.getX());
    rect.y = static_cast<int>(m_position.getY());
    rect.w = m_width;
    rect.h = m_height;

    SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, 255);
    SDL_RenderFillRect(pRenderer, &rect);

    int endX = static_cast<int>(m_position.getX() + m_power * std::cos(m_angle * M_PI / 180.0f));
    int endY = static_cast<int>(m_position.getY() - m_power * std::sin(m_angle * M_PI / 180.0f));
    SDL_RenderDrawLine(pRenderer,
        static_cast<int>(m_position.getX()),
        static_cast<int>(m_position.getY()),
        endX, endY);

    m_cannonBall->render(pRenderer);
}

void Cannon::clean() {
    m_cannonBall->clean();
    SDLGameObject::clean();
}

// 🔄 CHANGE: onCollision 메서드 제거 (기본 구현 사용)
void Cannon::handleInput() {
    InputHandler* inputHandler = InputHandler::Instance();

    if (inputHandler->isKeyDown(SDL_SCANCODE_DOWN)) {
        adjustAngle(-1.0f);
    }
    else if (inputHandler->isKeyDown(SDL_SCANCODE_UP)) {
        adjustAngle(1.0f);
    }

    if (inputHandler->isKeyDown(SDL_SCANCODE_LEFT)) {
        adjustPower(-1.0f);
    }
    else if (inputHandler->isKeyDown(SDL_SCANCODE_RIGHT)) {
        adjustPower(1.0f);
    }

    if (inputHandler->isKeyOneDown(SDL_SCANCODE_SPACE)) {
        std::cout << "Space key is pressed" << std::endl;
        shoot();
    }
}

void Cannon::adjustAngle(float angle) {
    m_angle += angle;
    if (m_angle < 0) m_angle = 0;
    if (m_angle > 90) m_angle = 90;
}

void Cannon::adjustPower(float power) {
    m_power += power;
    if (m_power < 0) m_power = 0;
    if (m_power > 100) m_power = 100;
}

void Cannon::shoot() {
    if (m_cannonBall->isVisible()) {
        return;
    }

    m_cannonBall->setPosition(m_position.getX(), m_position.getY());
    m_cannonBall->shoot(m_angle, m_power * 10);
}