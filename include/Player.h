#ifndef PLAYER_H
#define PLAYER_H

#include "SDLGameObject.h"
#include "InputHandler.h"

class Player : public SDLGameObject {
public:
    Player(const LoaderParams* pParams);
    virtual ~Player() {}

    virtual void update(float deltaTime) override;

    // 🆕 NEW: 충돌 처리 오버라이드
    virtual void onCollision(GameObject* other) override;

private:
    void handleInput();
};

#endif // PLAYER_H