#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "Component.h"
#include "Transform.h"

class GameObject {
public:
    GameObject(const std::string& name = "GameObject");
    virtual ~GameObject();

    // 기본 게임 루프 메서드
    void init();
    void update();
    void fixedUpdate();
    void render(SDL_Renderer* renderer);
    void destroy();

    // 컴포넌트 관리
    template<typename T>
    T* addComponent() {
        static_assert(std::is_base_of<Component, T>::value,
            "T must inherit from Component");

        T* component = new T();
        component->setGameObject(this);
        m_components.push_back(component);
        component->init();

        return component;
    }

    template<typename T>
    T* getComponent() {
        for (auto* component : m_components) {
            if (T* castComponent = dynamic_cast<T*>(component)) {
                return castComponent;
            }
        }
        return nullptr;
    }

    template<typename T>
    void removeComponent() {
        m_components.erase(
            std::remove_if(m_components.begin(), m_components.end(),
                [](Component* component) {
                    if (dynamic_cast<T*>(component)) {
                        component->destroy();
                        delete component;
                        return true;
                    }
                    return false;
                }
            ),
            m_components.end()
        );
    }

    // 게터/세터
    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    bool isActive() const { return m_active; }
    void setActive(bool active);

    Transform* getTransform() { return m_transform; }

private:
    std::string m_name;
    bool m_active{ true };
    std::vector<Component*> m_components;
    Transform* m_transform{ nullptr };  // Transform 컴포넌트 캐싱
};

#endif // GAMEOBJECT_H