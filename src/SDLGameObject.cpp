#include "SDLGameObject.h"

SDLGameObject::SDLGameObject(const LoaderParams* pParams)
    : GameObject(pParams) {
}

void SDLGameObject::update(float deltaTime) {
    GameObject::update(deltaTime);
}

void SDLGameObject::render(SDL_Renderer* pRenderer) {
    // 🔄 CHANGE: Vector2D 위치 사용
    TheTextureManager::Instance()->drawFrame(
        m_textureID,
        static_cast<int>(m_position.getX()),
        static_cast<int>(m_position.getY()),
        m_width, m_height,
        m_currentRow, m_currentFrame,
        pRenderer
    );
}

void SDLGameObject::clean() {
    // SDL 리소스 정리
}