#include "Player.h"
#include <SDL2/SDL.h>

Player::Player(const LoaderParams* pParams)
    : SDLGameObject(pParams) {
}

void Player::handleInput() {
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);

    const float ACCELERATION_X = 200.0f;
    const float ACCELERATION_Y = 200.0f;
    const float DECELERATION_FACTOR = 0.95;

    // 🆕 NEW: 가속도 초기화
    m_acceleration.setX(0);
    m_acceleration.setY(0);

    // 🆕 NEW: 수평 이동 처리
    if (keystate[SDL_SCANCODE_LEFT]) {
        m_velocity.setX(-ACCELERATION_X);
    }
    else if (keystate[SDL_SCANCODE_RIGHT]) {
        m_velocity.setX(ACCELERATION_X);
    }
    else {
        m_velocity.setX(m_velocity.getX() * DECELERATION_FACTOR);
    }

    // 🆕 NEW: 수직 이동 처리
    if (keystate[SDL_SCANCODE_UP]) {
        m_velocity.setY(-ACCELERATION_Y);
    }
    else if (keystate[SDL_SCANCODE_DOWN]) {
        m_velocity.setY(ACCELERATION_Y);
    }
    else {
        m_velocity.setY(m_velocity.getY() * DECELERATION_FACTOR);
    }
}

void Player::update(float deltaTime) {
    handleInput();
    SDLGameObject::update(deltaTime);
}