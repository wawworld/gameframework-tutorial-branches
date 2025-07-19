#include "TextureManager.h"

TextureManager* TextureManager::s_pInstance = nullptr;

// 🔄 CHANGE: TextureInfo 지원 추가
bool TextureManager::load(const std::string& fileName, const std::string& id,
    SDL_Renderer* pRenderer, const TextureInfo& info) {
    SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
    if (pTempSurface == nullptr) {
        std::cerr << "이미지 로드 실패: " << IMG_GetError() << std::endl;
        return false;
    }

    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
    SDL_FreeSurface(pTempSurface);

    if (pTexture != nullptr) {
        m_textureMap[id] = pTexture;

        // 🆕 NEW: TextureInfo 저장 (기본값이면 텍스처 크기로 설정)
        TextureInfo finalInfo = info;
        if (finalInfo.frameWidth == 0 || finalInfo.frameHeight == 0) {
            int w, h;
            SDL_QueryTexture(pTexture, nullptr, nullptr, &w, &h);
            finalInfo.frameWidth = w / finalInfo.columns;
            finalInfo.frameHeight = h / finalInfo.rows;
            finalInfo.totalFrames = finalInfo.rows * finalInfo.columns;
        }

        m_textureInfoMap[id] = finalInfo;
        return true;
    }

    return false;
}

void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;

    destRect.x = x;
    destRect.y = y;

    SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, nullptr, flip);
}

void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = width * currentFrame;
    srcRect.y = height * (currentRow - 1);
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;

    destRect.x = x;
    destRect.y = y;

    SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, nullptr, flip);
}

// 🔄 CHANGE: 메타데이터도 함께 제거
void TextureManager::clearFromTextureMap(std::string id) {
    SDL_DestroyTexture(m_textureMap[id]);
    m_textureMap.erase(id);
    m_textureInfoMap.erase(id);
}

// 🆕 NEW: 텍스처 접근자
SDL_Texture* TextureManager::getTexture(const std::string& id) {
    auto it = m_textureMap.find(id);
    return (it != m_textureMap.end()) ? it->second : nullptr;
}

// 🆕 NEW: 메타데이터 접근자
const TextureInfo* TextureManager::getTextureInfo(const std::string& id) {
    auto it = m_textureInfoMap.find(id);
    return (it != m_textureInfoMap.end()) ? &it->second : nullptr;
}