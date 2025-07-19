#ifndef CANNON_H
#define CANNON_H

#include "SDLGameObject.h"

class CannonBall;

class Cannon : public SDLGameObject {
public:
    Cannon(const LoaderParams* pParams);
    virtual ~Cannon();

    virtual void update(float deltaTime) override;
    virtual void render(SDL_Renderer* pRenderer) override;
    virtual void clean() override;
    // virtual void onCollision(GameObject* other) override;

private:
    void handleInput();
    void adjustAngle(float angle);
    void adjustPower(float power);
    void shoot();

    float m_angle;
    float m_power;
    CannonBall* m_cannonBall;
};

#endif // CANNON_H