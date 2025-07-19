#include "Player.h"
#include <SDL2/SDL.h>
#include <iostream>

Player::Player(const LoaderParams* pParams)
    : SDLGameObject(pParams) {
}

void Player::update(float deltaTime) {
    handleInput(); // 🆕 NEW: 사용자 입력 처리
    SDLGameObject::update(deltaTime); // 부모 update 함수 호출
}

void Player::handleInput() {
    InputHandler* inputHandler = InputHandler::Instance();

    // 🆕 NEW: 연속 입력 처리 - 이동
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

    // 🆕 NEW: 단일 입력 처리 - 액션
    if (inputHandler->isKeyOneDown(SDL_SCANCODE_SPACE))
    {
        std::cerr << "SPACE" << std::endl;
    }
}