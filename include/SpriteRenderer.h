#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Component.h"
#include "Transform.h"
#include <SDL2/SDL.h>
#include <string>

class TextureInfo;

class SpriteRenderer : public Component {
public:
    SpriteRenderer();
    virtual ~SpriteRenderer();

    // Component 인터페이스 구현
    void init() override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void destroy() override;

    // 텍스처 관리
    void setTextureID(const std::string& textureID);

    // 프레임 설정
    void setFrameSize(int width, int height);
    void setCurrentFrame(int row, int col);
    void setFrameCount(int count) { m_frameCount = count; }

    // 애니메이션 설정
    void setAnimated(bool animated) { m_isAnimated = animated; }
    void setAnimationSpeed(float speed) { m_animationSpeed = speed; }

    // 렌더링 속성
    void setFlip(SDL_RendererFlip flip) { m_flip = flip; }
    void setColor(SDL_Color color) { m_color = color; }
    void setAlpha(Uint8 alpha) { m_color.a = alpha; }
    void setBlendMode(SDL_BlendMode blendMode) { m_blendMode = blendMode; }

private:
    // 컴포넌트 참조
    Transform* m_transform;

    // 텍스처 정보
    std::string m_textureID;
    SDL_Texture* m_texture;
    const TextureInfo* m_textureInfo;

    // 프레임 정보
    int m_frameWidth;
    int m_frameHeight;
    int m_currentRow;
    int m_currentCol;
    int m_frameCount;

    // 애니메이션 정보
    bool m_isAnimated;
    float m_animationSpeed;
    float m_animationTimer;

    // 렌더링 속성
    SDL_RendererFlip m_flip;
    SDL_Color m_color;
    SDL_BlendMode m_blendMode;

    // 내부 유틸리티 함수
    void updateAnimation(float deltaTime);
    void calculateSourceRect(SDL_Rect& sourceRect) const;
    void calculateDestRect(SDL_Rect& destRect) const;
};

#endif // SPRITE_RENDERER_H