#ifndef SDLGAMEOBJECT_H
#define SDLGAMEOBJECT_H

#include "GameObject.h"
#include "TextureManager.h"

class SDLGameObject : public GameObject {
public:
    SDLGameObject(const LoaderParams* pParams);
    virtual ~SDLGameObject() {}

    virtual void update(float deltaTime) override;
    virtual void render(SDL_Renderer* pRenderer) override;
    virtual void clean() override;
};

#endif // SDLGAMEOBJECT_H