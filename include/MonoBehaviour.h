#ifndef MONOBEHAVIOUR_H
#define MONOBEHAVIOUR_H

#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

class MonoBehaviour : public Component {
public:
    // Component 인터페이스 구현
    void init() override final;
    void update() override final;
    void fixedUpdate() override final;
    void render(SDL_Renderer* renderer) override;
    void destroy() override final;

    // MonoBehaviour 가상 함수들
    virtual void Awake() {}
    virtual void Start() {}
    virtual void Update() {}
    virtual void FixedUpdate() {}
    virtual void OnDestroy() {}

    // 상태 변경 관리
    void setActive(bool active);
    virtual void OnEnable() {}
    virtual void OnDisable() {}

    // 유틸리티 함수들
    Transform* getTransform() const { return m_transform; }

    template<typename T>
    T* getComponent() {
        if (auto gameObj = getGameObject()) {
            return gameObj->template getComponent<T>();
        }
        return nullptr;
    }


private:
    Transform* m_transform = nullptr;
    bool m_started = false;
};

#endif // MONOBEHAVIOUR_H