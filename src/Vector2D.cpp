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