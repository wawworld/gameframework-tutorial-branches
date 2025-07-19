#include "Enemy.h"

// 🆕 NEW: 기본 클래스 생성자 호출 및 Enemy 초기화
Enemy::Enemy(const std::string& textureID, int x, int y, int width, int height)
    : GameObject(textureID, x, y, width, height), m_direction(1) {
    // 🆕 NEW: 초기 이동 방향을 오른쪽(1)으로 설정
}

// 🆕 NEW: Enemy 전용 업데이트 로직
void Enemy::update() {
    // 🆕 NEW: 방향에 따른 X축 위치 변경
    m_x += m_direction;

    // 🆕 NEW: 화면 경계에 도달하면 방향을 반전
    if (m_x + m_width > 640 || m_x < 0) {
        m_direction = -m_direction;
    }

    // 🆕 NEW: 기본 클래스의 업데이트 호출 (속도 기반 이동 적용)
    GameObject::update();
}