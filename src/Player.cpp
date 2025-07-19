#include "Player.h"
#include <SDL2/SDL.h>

Player::Player(const LoaderParams* pParams)
    : SDLGameObject(pParams) {
    // Player 특화 초기화가 필요한 경우 여기에 추가
}

void Player::handleInput() {
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);

    // 🔄 CHANGE: 매 프레임마다 속도 초기화
    m_velocityX = 0;
    m_velocityY = 0;

    // 🔄 CHANGE: 키보드 입력에 따른 플레이어 이동
    if (keystate[SDL_SCANCODE_UP]) {
        m_velocityY = -1;
    }
    else if (keystate[SDL_SCANCODE_DOWN]) {
        m_velocityY = 1;
    }

    if (keystate[SDL_SCANCODE_LEFT]) {
        m_velocityX = -1;
    }
    else if (keystate[SDL_SCANCODE_RIGHT]) {
        m_velocityX = 1;
    }
}

void Player::update() {
    handleInput();              // 🔄 CHANGE: 입력 처리 먼저 수행
    SDLGameObject::update();    // 🔄 CHANGE: 상위 클래스 업데이트 호출
}