#include "Player.h"
#include <SDL2/SDL.h>

// 🆕 NEW: 기본 클래스 생성자 호출
Player::Player(const std::string& textureID, int x, int y, int width, int height)
    : GameObject(textureID, x, y, width, height) {
    // Player 특화 초기화가 필요한 경우 여기에 추가
}

// 🆕 NEW: 키보드 입력 처리 전용 메서드
void Player::handleInput() {
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);

    // 🆕 NEW: 매 프레임마다 속도 초기화
    m_velocityX = 0;
    m_velocityY = 0;

    // 🆕 NEW: 키보드 입력에 따른 플레이어 이동
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

// 🆕 NEW: Player 전용 업데이트 로직
void Player::update() {
    handleInput();              // 🆕 NEW: 입력 처리 먼저 수행
    GameObject::update();       // 🆕 NEW: 기본 클래스의 업데이트 호출
}