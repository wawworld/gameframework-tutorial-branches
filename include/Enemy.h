#ifndef ENEMY_H
#define ENEMY_H

#include "SDLGameObject.h"

class Enemy : public SDLGameObject {
public:
    Enemy(const LoaderParams* pParams);
    virtual ~Enemy() {}

    virtual void update(float deltaTime) override;
};

#endif // ENEMY_H