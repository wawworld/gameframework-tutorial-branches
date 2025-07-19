#ifndef ENEMY_H
#define ENEMY_H

#include "SDLGameObject.h"

class Enemy : public SDLGameObject {
public:
    Enemy(const LoaderParams* pParams);
    virtual ~Enemy() {}

    virtual void update(float deltaTime) override;

    // 🆕 NEW: 충돌 처리 오버라이드
    virtual void onCollision(GameObject* other) override;

private:
    void handleInput();
};

#endif // ENEMY_H