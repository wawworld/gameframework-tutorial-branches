#include "MonoBehaviour.h"
#include "GameObject.h"

void MonoBehaviour::init()
{
    m_transform = getGameObject()->getComponent<Transform>();
    Awake();
}

void MonoBehaviour::update()
{
    if (!isActive()) return;

    if (!m_started)
    {
        Start();
        m_started = true;
    }
    Update();
}

void MonoBehaviour::fixedUpdate()
{
    if (isActive())
    {
        FixedUpdate();
    }
}

void MonoBehaviour::render(SDL_Renderer* renderer)
{
    // 기본적으로 비어있음
}

void MonoBehaviour::destroy()
{
    OnDestroy();
}

void MonoBehaviour::setActive(bool active)
{
    if (m_active != active)
    {
        m_active = active;
        if (m_active)
            OnEnable();
        else
            OnDisable();
    }
}