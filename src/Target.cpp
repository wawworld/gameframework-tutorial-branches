#include "Target.h"
#include "CollisionManager.h"
#include <random>
#include <ctime>

Target::Target(const LoaderParams* pParams) : SDLGameObject(pParams), m_isVisible(true) {
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> distX(200, 200 + 50 * 9);
    std::uniform_int_distribution<int> distY(50, 50 + 50 * 6);

    m_position.setX(distX(rng));
    m_position.setY(distY(rng));
}

void Target::update(float deltaTime) {
    // 타겟은 정적 객체로 업데이트 불필요
}

void Target::render(SDL_Renderer* pRenderer) {
    if (!m_isVisible) return;

    SDL_Rect rect;
    rect.x = static_cast<int>(m_position.getX());
    rect.y = static_cast<int>(m_position.getY());
    rect.w = m_width;
    rect.h = m_height;

    SDL_SetRenderDrawColor(pRenderer, 0, 255, 255, 255);
    SDL_RenderFillRect(pRenderer, &rect);
}

void Target::clean() {
    SDLGameObject::clean();
}

void Target::onCollision(GameObject* other) {
    SDLGameObject* pOther = dynamic_cast<SDLGameObject*>(other);
    if (pOther != nullptr && pOther->getTextureID() == "CannonBall") {
        m_isVisible = false;
        CollisionManager::Instance()->removeGameObject(this);
    }
}