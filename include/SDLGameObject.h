#ifndef SDLGAMEOBJECT_H
#define SDLGAMEOBJECT_H

#include "GameObject.h"
#include "LoaderParams.h"

class SDLGameObject : public GameObject {
public:
    SDLGameObject(const LoaderParams* pParams);
    virtual ~SDLGameObject() {}

    // 🆕 NEW: 순수 가상 함수들의 구현
    virtual void update() override;
    virtual void render(SDL_Renderer* pRenderer) override;
    virtual void clean() override;
};

#endif // SDLGAMEOBJECT_H