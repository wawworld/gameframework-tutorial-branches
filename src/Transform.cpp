#include "Transform.h"

Transform::Transform()
    : m_position(0, 0)
    , m_scale(1, 1)
    , m_rotation(0) {
}

void Transform::init() {
    // Transform 초기화 로직이 필요한 경우 여기에 구현
}

void Transform::update() {
    // Transform 업데이트 로직이 필요한 경우 여기에 구현
}

void Transform::render(SDL_Renderer* renderer) {
    // Transform은 시각적 렌더링이 필요 없음
}

void Transform::destroy() {
    // Transform 정리 로직이 필요한 경우 여기에 구현
}

void Transform::setPosition(const Vector2D& position) {
    m_position = position;
}

void Transform::translate(const Vector2D& offset) {
    m_position += offset;
}

Vector2D Transform::getPosition() const {
    return m_position;
}

void Transform::setScale(const Vector2D& scale) {
    m_scale = scale;
}

void Transform::scale(const Vector2D& factor) {
    m_scale.setX(m_scale.getX() * factor.getX());
    m_scale.setY(m_scale.getY() * factor.getY());
}

Vector2D Transform::getScale() const {
    return m_scale;
}

void Transform::setRotation(float rotation) {
    m_rotation = rotation;
    normalizeRotation();
}

void Transform::rotate(float angle) {
    m_rotation += angle;
    normalizeRotation();
}

float Transform::getRotation() const {
    return m_rotation;
}

void Transform::normalizeRotation() {
    while (m_rotation >= 360.0f) {
        m_rotation -= 360.0f;
    }
    while (m_rotation < 0.0f) {
        m_rotation += 360.0f;
    }
}