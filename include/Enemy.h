#ifndef ENEMY_H
#define ENEMY_H

#include "SDLGameObject.h"

class Enemy : public SDLGameObject {
public:
    // 🔄 CHANGE: LoaderParams를 받는 생성자
    Enemy(const LoaderParams* pParams);
    virtual ~Enemy() {}

    // 🔄 CHANGE: SDLGameObject의 메서드 오버라이드
    virtual void update() override;

private:
    // 🔄 CHANGE: Enemy 전용 멤버 변수
    int m_direction;
};

#endif // ENEMY_H