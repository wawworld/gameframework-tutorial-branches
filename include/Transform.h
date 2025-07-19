#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include "Vector2D.h"
#include <SDL2/SDL.h>

class Transform : public Component {
public:
    Transform();

    // Component 인터페이스 구현
    void init() override;
    void update() override;
    void render(SDL_Renderer* renderer) override;
    void destroy() override;

    // Transform 위치 관련
    void setPosition(const Vector2D& position);
    void translate(const Vector2D& offset);
    Vector2D getPosition() const;

    // Transform 크기 관련
    void setScale(const Vector2D& scale);
    void scale(const Vector2D& factor);
    Vector2D getScale() const;

    // Transform 회전 관련
    void setRotation(float rotation);
    void rotate(float angle);
    float getRotation() const;

private:
    Vector2D m_position;    // 위치
    Vector2D m_scale;       // 크기
    float m_rotation;       // 회전 (각도)

    // 회전 각도를 0~360 범위로 정규화
    void normalizeRotation();
};

#endif // TRANSFORM_H