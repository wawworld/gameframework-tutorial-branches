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

    virtual void update(float deltaTime);
    virtual void render(SDL_Renderer* pRenderer) = 0;
    virtual void clean() = 0;

    // 🆕 NEW: 충돌 처리 가상 함수
    virtual void onCollision(GameObject* other);

    void setVelocity(int x, int y);
    void setPosition(int x, int y);
    std::string getTextureID() const { return m_textureID; }

    // 🆕 NEW: 충돌 검사용 접근자 메서드
    Vector2D getPosition() const { return m_position; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

protected:
    std::string m_textureID;
    int m_width, m_height;
    int m_currentFrame, m_currentRow;

    Vector2D m_position;
    Vector2D m_velocity;
    Vector2D m_acceleration;
};

#endif // GAMEOBJECT_H