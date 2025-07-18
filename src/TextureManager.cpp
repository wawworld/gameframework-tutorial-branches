#include "TextureManager.h"

// 🆕 NEW: 정적 멤버 변수 초기화
TextureManager* TextureManager::s_pInstance = nullptr;

bool TextureManager::load(std::string fileName, std::string id, SDL_Renderer* pRenderer) {
    // 🆕 NEW: 이미지 파일을 Surface로 로드
    SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());
    if (pTempSurface == nullptr) {
        std::cerr << "이미지 로드 실패: " << IMG_GetError() << std::endl;
        return false;
    }

    // 🆕 NEW: Surface를 Texture로 변환
    SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
    SDL_FreeSurface(pTempSurface);

    // 🆕 NEW: 텍스처를 map에 저장
    if (pTexture != nullptr) {
        m_textureMap[id] = pTexture;
        return true;
    }

    return false;
}

void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;

    // 🆕 NEW: 전체 텍스처를 원본으로 사용
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;

    // 🆕 NEW: 대상 위치 설정
    destRect.x = x;
    destRect.y = y;

    // 🆕 NEW: 텍스처 렌더링 (flip 옵션 포함)
    SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, nullptr, flip);
}

void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;

    // 🆕 NEW: 스프라이트 시트에서 특정 프레임 선택
    srcRect.x = width * currentFrame;           // 프레임 위치 계산
    srcRect.y = height * (currentRow - 1);      // 행 위치 계산 (1부터 시작)
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;

    // 🆕 NEW: 대상 위치 설정
    destRect.x = x;
    destRect.y = y;

    // 🆕 NEW: 선택된 프레임 렌더링
    SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, nullptr, flip);
}

void TextureManager::clearFromTextureMap(std::string id) {
    // 🆕 NEW: 텍스처 메모리 해제
    SDL_DestroyTexture(m_textureMap[id]);

    // 🆕 NEW: map에서 항목 제거
    m_textureMap.erase(id);
}