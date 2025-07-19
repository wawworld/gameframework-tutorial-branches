#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"

class Enemy : public GameObject {
public:
    // 🆕 NEW: 기본 클래스 생성자 호출하는 생성자
    Enemy(const std::string& textureID, int x, int y, int width, int height);

    // 🆕 NEW: 가상 소멸자
    virtual ~Enemy() {}

    // 🆕 NEW: 적의 자동 이동 동작 구현
    virtual void update() override;

private:
    // 🆕 NEW: Enemy 전용 멤버 변수
    int m_direction;                    // 이동 방향 (1: 오른쪽, -1: 왼쪽)
};

#endif // ENEMY_H