#ifndef COMPONENT_H
#define COMPONENT_H

#include <SDL2/SDL.h>

class GameObject;

class Component {
public:
    virtual ~Component() = default;

    // 핵심 가상 함수들
    virtual void init() = 0;                    // 초기화
    virtual void render(SDL_Renderer* renderer) = 0;  // 렌더링
    virtual void destroy() = 0;                 // 정리

    virtual void update() {}                    // 일반 업데이트
    virtual void fixedUpdate() {}               // 물리 업데이트

    // GameObject 연결 관리
    void setGameObject(GameObject* gameObject) { m_gameObject = gameObject; }
    GameObject* getGameObject() const { return m_gameObject; }

    // 활성화 상태 관리
    bool isActive() const { return m_active; }
    void setActive(bool active) { m_active = active; }

protected:
    GameObject* m_gameObject = nullptr;
    bool m_active = true;
};

#endif // COMPONENT_H