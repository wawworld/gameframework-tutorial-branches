#include "SpriteRenderer.h"
#include "GameObject.h"
#include "TextureManager.h"
#include "GameTime.h"
#include <iostream>
#include <algorithm>

SpriteRenderer::SpriteRenderer() :
    m_transform(nullptr),
    m_textureID(""),
    m_frameCount(1),
    m_currentRow(0),
    m_currentCol(0),
    m_isAnimated(false),
    m_animationSpeed(1.0f),
    m_animationTimer(0.0f),
    m_flip(SDL_FLIP_NONE),
    m_color({ 255, 255, 255, 255 }),
    m_blendMode(SDL_BLENDMODE_BLEND),
    m_texture(nullptr),
    m_textureInfo(nullptr)
{
}

SpriteRenderer::~SpriteRenderer() {
    // 소멸자 구현
}

void SpriteRenderer::init()
{
    // Transform 컴포넌트 가져오기
    m_transform = getGameObject()->getComponent<Transform>();
    if (!m_transform) {
        std::cerr << "SpriteRenderer requires Transform component" << std::endl;
    }
}

void SpriteRenderer::update()
{
    if (!m_isAnimated || !isActive()) {
        return;
    }

    updateAnimation(GameTime::getDeltaTime());
}

void SpriteRenderer::render(SDL_Renderer* renderer)
{
    if (!isActive() || !m_texture || !m_transform) {
        return;
    }

    SDL_Rect sourceRect;
    SDL_Rect destRect;

    calculateSourceRect(sourceRect);
    calculateDestRect(destRect);

    // 색상 설정
    SDL_SetTextureColorMod(m_texture, m_color.r, m_color.g, m_color.b);
    SDL_SetTextureAlphaMod(m_texture, m_color.a);

    // 렌더링
    SDL_RenderCopyEx(
        renderer,
        m_texture,
        &sourceRect,
        &destRect,
        m_transform->getRotation(),
        nullptr,  // 회전 중심점 (null이면 중앙)
        m_flip
    );
}

void SpriteRenderer::destroy()
{
    // 리소스 정리가 필요한 경우 여기서 처리
    m_transform = nullptr;
}

void SpriteRenderer::setTextureID(const std::string& id)
{
    m_textureID = id;

    // TextureManager에서 텍스처와 텍스처 정보 가져오기
    m_texture = TextureManager::Instance()->getTexture(m_textureID);
    m_textureInfo = TextureManager::Instance()->getTextureInfo(m_textureID);

    if (!m_texture || !m_textureInfo) {
        std::cerr << "Failed to get texture or texture info for ID: " << id << std::endl;
        return;
    }

    // 프레임 수가 설정되지 않았다면 텍스처 정보 기반으로 설정
    if (m_frameCount == 1) {
        m_frameCount = m_textureInfo->columns;
    }
}

void SpriteRenderer::setFrameSize(int width, int height)
{
    m_frameWidth = width;
    m_frameHeight = height;
}

void SpriteRenderer::setCurrentFrame(int row, int col)
{
    if (!m_textureInfo) {
        return;
    }

    // 범위 검사
    m_currentRow = std::min(std::max(0, row), m_textureInfo->rows - 1);
    m_currentCol = std::min(std::max(0, col), m_textureInfo->columns - 1);
}

void SpriteRenderer::updateAnimation(float deltaTime)
{
    m_animationTimer += deltaTime * m_animationSpeed;

    // 다음 프레임으로 진행
    if (m_animationTimer >= 1.0f) {
        m_animationTimer = 0.0f;
        m_currentCol = (m_currentCol + 1) % m_frameCount;
    }
}

void SpriteRenderer::calculateSourceRect(SDL_Rect& sourceRect) const
{
    if (!m_textureInfo) {
        sourceRect = { 0, 0, 0, 0 };
        return;
    }

    sourceRect.x = m_currentCol * m_textureInfo->frameWidth;
    sourceRect.y = m_currentRow * m_textureInfo->frameHeight;
    sourceRect.w = m_textureInfo->frameWidth;
    sourceRect.h = m_textureInfo->frameHeight;
}

void SpriteRenderer::calculateDestRect(SDL_Rect& destRect) const
{
    if (!m_transform || !m_textureInfo) {
        destRect = { 0, 0, 0, 0 };
        return;
    }

    Vector2D position = m_transform->getPosition();
    Vector2D scale = m_transform->getScale();

    destRect.x = position.getX();
    destRect.y = position.getY();
    destRect.w = m_textureInfo->frameWidth * scale.getX();
    destRect.h = m_textureInfo->frameHeight * scale.getY();
}