#ifndef CANNONBALL_H
#define CANNONBALL_H

#include "SDLGameObject.h"

class CannonBall : public SDLGameObject {
public:
    CannonBall(const LoaderParams* pParams);
    virtual ~CannonBall() {}

    virtual void update(float deltaTime) override;
    virtual void render(SDL_Renderer* pRenderer) override;
    virtual void clean() override;
    virtual void onCollision(GameObject* other) override;

    void shoot(float angle, float power);
    bool isVisible() const { return m_isVisible; }

private:
    bool m_isVisible;
    float m_velocityX;
    float m_velocityY;
    float m_timeElapsed;
};

#endif // CANNONBALL_H