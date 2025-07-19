#include "SDLGameObject.h"
#include "TextureManager.h"

SDLGameObject::SDLGameObject(const LoaderParams* pParams)
    : GameObject(pParams) {
    // SDL 특화 초기화가 필요한 경우 여기에 추가
}

void SDLGameObject::update() {
    // 🆕 NEW: 속도 기반 위치 업데이트
    m_x += m_velocityX;
    m_y += m_velocityY;
}

void SDLGameObject::render(SDL_Renderer* pRenderer) {
    // 🆕 NEW: TextureManager를 통한 프레임 렌더링
    TheTextureManager::Instance()->drawFrame(m_textureID, m_x, m_y,
        m_width, m_height,
        m_currentRow, m_currentFrame,
        pRenderer);
}

void SDLGameObject::clean() {
    // 🆕 NEW: SDL 리소스 정리 (필요시 구현)
}