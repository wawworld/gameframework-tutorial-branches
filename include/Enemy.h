#ifndef ENEMY_H
#define ENEMY_H

#include "SDLGameObject.h"

class Enemy : public SDLGameObject {
public:
    Enemy(const LoaderParams* pParams);
    virtual ~Enemy() {}

    virtual void update(float deltaTime) override;

private:
    void handleInput();  // 🆕 NEW: 마우스 입력 처리 메서드 추가
};

#endif // ENEMY_H