#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <SDL2/SDL.h>
#include "LoaderParams.h"
#include "Vector2D.h"

class GameObject {
public:
    GameObject(const LoaderParams* pParams);
    virtual ~GameObject() {}

    // 🔄 CHANGE: deltaTime 매개변수 추가
    virtual void update(float deltaTime);
    virtual void render(SDL_Renderer* pRenderer) = 0;
    virtual void clean() = 0;

    void setVelocity(int x, int y);
    void setPosition(int x, int y);
    std::string getTextureID() const { return m_textureID; }

protected:
    std::string m_textureID;
    int m_width, m_height;
    int m_currentFrame, m_currentRow;

    // 🆕 NEW: Vector2D 멤버 변수들
    Vector2D m_position;
    Vector2D m_velocity;
    Vector2D m_acceleration;
};

#endif // GAMEOBJECT_H