#include "Enemy.h"

Enemy::Enemy(const LoaderParams* pParams)
    : SDLGameObject(pParams), m_direction(1) {
    // 🔄 CHANGE: 초기 이동 방향을 오른쪽(1)으로 설정
}

void Enemy::update() {
    // 🔄 CHANGE: 방향에 따른 X축 위치 변경
    m_x += m_direction;

    // 🔄 CHANGE: 화면 경계에 도달하면 방향을 반전
    if (m_x + m_width > 640 || m_x < 0) {
        m_direction = -m_direction;
    }

    // 🔄 CHANGE: 상위 클래스의 업데이트 호출
    SDLGameObject::update();
}