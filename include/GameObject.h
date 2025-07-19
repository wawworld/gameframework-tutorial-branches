#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <SDL2/SDL.h>
#include "LoaderParams.h"

class GameObject {
public:
    // 🆕 NEW: LoaderParams를 받는 생성자
    GameObject(const LoaderParams* pParams);

    // 🆕 NEW: 가상 소멸자
    virtual ~GameObject() {}

    // 🆕 NEW: 순수 가상 함수들 - 자식 클래스에서 반드시 구현
    virtual void update() = 0;
    virtual void render(SDL_Renderer* pRenderer) = 0;
    virtual void clean() = 0;

    // 🆕 NEW: 공통 유틸리티 함수들
    void setVelocity(int x, int y);
    void setPosition(int x, int y);
    std::string getTextureID() const { return m_textureID; }

protected:
    // 🆕 NEW: 자식 클래스에서 접근 가능한 멤버들
    std::string m_textureID;
    int m_x, m_y, m_width, m_height;
    int m_velocityX, m_velocityY;
    int m_currentFrame, m_currentRow;
};

#endif // GAMEOBJECT_H