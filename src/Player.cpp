#include "Player.h"
#include <SDL2/SDL.h>
#include <iostream>

Player::Player(const LoaderParams* pParams)
    : SDLGameObject(pParams) {
}

void Player::update(float deltaTime) {
    handleInput();
    SDLGameObject::update(deltaTime);
}

void Player::handleInput() {
    InputHandler* inputHandler = InputHandler::Instance();

    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT))
    {
        m_velocity.setX(100);
    }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT))
    {
        m_velocity.setX(-100);
    }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP))
    {
        m_velocity.setY(-100);
    }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN))
    {
        m_velocity.setY(100);
    }

    if (inputHandler->isKeyOneDown(SDL_SCANCODE_SPACE))
    {
        std::cerr << "SPACE" << std::endl;
    }
}

// 🆕 NEW: 플레이어 충돌 처리 구현
void Player::onCollision(GameObject* other) {
    std::cout << "Player collided with object!" << std::endl;

    // 충돌 시 플레이어 이동 중지
    m_velocity.setX(0);
    m_velocity.setY(0);
}