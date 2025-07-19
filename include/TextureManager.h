#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <string>
#include <iostream>

class TextureManager {
public:
    // 🆕 NEW: 싱글톤 인스턴스 접근 함수
    static TextureManager* Instance() {
        if (s_pInstance == nullptr) {
            s_pInstance = new TextureManager();
        }
        return s_pInstance;
    }

    // 🆕 NEW: 텍스처 관리 함수들
    bool load(std::string fileName, std::string id, SDL_Renderer* pRenderer);
    void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void clearFromTextureMap(std::string id);

private:
    // 🆕 NEW: 싱글톤 패턴 구현
    TextureManager() {}
    ~TextureManager() {}
    static TextureManager* s_pInstance;

    // 🆕 NEW: 텍스처 저장 컨테이너
    std::map<std::string, SDL_Texture*> m_textureMap;
};

// 🆕 NEW: 편의성을 위한 매크로 정의
typedef TextureManager TheTextureManager;

#endif // TEXTUREMANAGER_H