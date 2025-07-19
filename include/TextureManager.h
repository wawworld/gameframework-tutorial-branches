#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <map>
#include <string>
#include <iostream>

struct TextureInfo {
    int frameWidth;
    int frameHeight;
    int rows;
    int columns;
    int totalFrames;

    TextureInfo(int fw = 0, int fh = 0, int r = 1, int c = 1)
        : frameWidth(fw), frameHeight(fh), rows(r), columns(c) {
        totalFrames = rows * columns;
    }
};

class TextureManager {
public:
    static TextureManager* Instance() {
        if (s_pInstance == nullptr) {
            s_pInstance = new TextureManager();
        }
        return s_pInstance;
    }

    bool load(const std::string& fileName, const std::string& id,
        SDL_Renderer* pRenderer, const TextureInfo& info = TextureInfo());

    void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void clearFromTextureMap(std::string id);

    SDL_Texture* getTexture(const std::string& id);
    const TextureInfo* getTextureInfo(const std::string& id);

private:
    TextureManager() {}
    ~TextureManager() {}
    static TextureManager* s_pInstance;

    std::map<std::string, SDL_Texture*> m_textureMap;
    std::map<std::string, TextureInfo> m_textureInfoMap;
};

typedef TextureManager TheTextureManager;

#endif // TEXTUREMANAGER_H