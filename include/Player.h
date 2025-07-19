#ifndef PLAYER_H
#define PLAYER_H

#include "SDLGameObject.h"

class Player : public SDLGameObject {
public:
    Player(const LoaderParams* pParams);
    virtual ~Player() {}

    virtual void update(float deltaTime) override;
    virtual void handleInput();
};

#endif // PLAYER_H