#include "GameObject.h"
#include "TextureManager.h"

// 🆕 NEW: 멤버 초기화 리스트를 사용한 생성자
GameObject::GameObject(const std::string& textureID, int x, int y, int width, int height)
    : m_textureID(textureID), m_x(x), m_y(y), m_width(width), m_height(height),
    m_velocityX(0), m_velocityY(0), m_currentFrame(0), m_currentRow(1) {
}

// 🆕 NEW: 기본 업데이트 로직 - 속도에 따른 위치 변경
void GameObject::update() {
    m_x += m_velocityX;
    m_y += m_velocityY;
}

// 🆕 NEW: 기본 렌더링 로직 - TextureManager를 통한 프레임 출력
void GameObject::render(SDL_Renderer* pRenderer) {
    TextureManager::Instance()->drawFrame(m_textureID, m_x, m_y,
        m_width, m_height,
        m_currentRow, m_currentFrame,
        pRenderer);
}

// 🆕 NEW: 기본 정리 함수 - 필요시 파생 클래스에서 확장
void GameObject::clean() {
    // 기본적으로는 특별한 정리 작업 없음
    // 파생 클래스에서 필요에 따라 오버라이드
}

// 🆕 NEW: 속도 설정 유틸리티
void GameObject::setVelocity(int x, int y) {
    m_velocityX = x;
    m_velocityY = y;
}

// 🆕 NEW: 위치 설정 유틸리티
void GameObject::setPosition(int x, int y) {
    m_x = x;
    m_y = y;
}