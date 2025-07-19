#include "GameObject.h"

GameObject::GameObject(const std::string& name)
    : m_name(name)
    , m_active(true) {
    // Transform 컴포넌트 자동 생성 및 캐싱
    m_transform = addComponent<Transform>();
}

GameObject::~GameObject() {
    destroy();

    // 모든 컴포넌트 제거
    for (auto component : m_components) {
        if (component) {
            delete component;
        }
    }
    m_components.clear();
}

void GameObject::init() {
    for (auto component : m_components) {
        if (component && component->isActive()) {
            component->init();
        }
    }
}

void GameObject::update() {
    if (!m_active) return;

    for (auto component : m_components) {
        if (component && component->isActive()) {
            component->update();
        }
    }
}

void GameObject::fixedUpdate() {
    if (!m_active) return;

    for (auto component : m_components) {
        if (component && component->isActive()) {
            component->fixedUpdate();
        }
    }
}

void GameObject::render(SDL_Renderer* renderer) {
    if (!m_active) return;

    for (auto component : m_components) {
        if (component && component->isActive()) {
            component->render(renderer);
        }
    }
}

void GameObject::destroy() {
    for (auto component : m_components) {
        if (component) {
            component->destroy();
        }
    }
}

void GameObject::setActive(bool active) {
    m_active = active;

    // 모든 컴포넌트의 활성화 상태도 변경
    for (auto component : m_components) {
        if (component) {
            component->setActive(active);
        }
    }
}