#ifndef TARGET_H
#define TARGET_H

#include "SDLGameObject.h"

class Target : public SDLGameObject {
public:
    Target(const LoaderParams* pParams);
    virtual ~Target() {}

    virtual void update(float deltaTime) override;
    virtual void render(SDL_Renderer* pRenderer) override;
    virtual void clean() override;
    virtual void onCollision(GameObject* other) override;

private:
    bool m_isVisible;
};

#endif // TARGET_H