#ifndef PLAYER_H
#define PLAYER_H

#include "SDLGameObject.h"
#include "InputHandler.h"  // 🆕 NEW: InputHandler 헤더 추가

class Player : public SDLGameObject {
public:
    Player(const LoaderParams* pParams);
    virtual ~Player() {}

    virtual void update(float deltaTime) override;

private:
    void handleInput();  // 🆕 NEW: 입력 처리 메서드 추가
};

#endif // PLAYER_H