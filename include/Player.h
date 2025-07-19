#ifndef PLAYER_H
#define PLAYER_H

#include "SDLGameObject.h"

class Player : public SDLGameObject {
public:
    // 🔄 CHANGE: LoaderParams를 받는 생성자
    Player(const LoaderParams* pParams);
    virtual ~Player() {}

    // 🔄 CHANGE: SDLGameObject의 메서드들을 오버라이드
    virtual void update() override;
    virtual void handleInput();
};

#endif // PLAYER_H