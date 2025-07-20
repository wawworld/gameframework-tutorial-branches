#include "Vector2D.h"

Vector2D Vector2D::operator+(const Vector2D& v2) const {
    return Vector2D(m_x + v2.m_x, m_y + v2.m_y);
}

Vector2D Vector2D::operator-(const Vector2D& v2) const {
    return Vector2D(m_x - v2.m_x, m_y - v2.m_y);
}

Vector2D Vector2D::operator*(float scalar) const {
    return Vector2D(m_x * scalar, m_y * scalar);
}

Vector2D Vector2D::operator/(float scalar) const {
    return Vector2D(m_x / scalar, m_y / scalar);
}

// 🆕 NEW: 단항 마이너스 연산자
Vector2D Vector2D::operator-() const {
    return Vector2D(-m_x, -m_y);
}

Vector2D& Vector2D::operator*=(float scalar) {
    m_x *= scalar;
    m_y *= scalar;
    return *this;
}

Vector2D& Vector2D::operator/=(float scalar) {
    m_x /= scalar;
    m_y /= scalar;
    return *this;
}

// 🆕 NEW: 비교 연산자
bool Vector2D::operator==(const Vector2D& other) const {
    const float EPSILON = 0.0001f;
    return std::abs(m_x - other.m_x) < EPSILON && std::abs(m_y - other.m_y) < EPSILON;
}

bool Vector2D::operator!=(const Vector2D& other) const {
    return !(*this == other);
}

void Vector2D::normalize() {
    float len = length();
    if (len > 0) {
        m_x /= len;
        m_y /= len;
    }
    else {
        m_x = 0;
        m_y = 0;
    }
}

// 🆕 NEW: 정규화된 벡터 반환
Vector2D Vector2D::normalized() const {
    Vector2D result = *this;
    result.normalize();
    return result;
}

Vector2D& operator+=(Vector2D& v1, const Vector2D& v2) {
    v1.m_x += v2.m_x;
    v1.m_y += v2.m_y;
    return v1;
}

Vector2D& operator-=(Vector2D& v1, const Vector2D& v2) {
    v1.m_x -= v2.m_x;
    v1.m_y -= v2.m_y;
    return v1;
}