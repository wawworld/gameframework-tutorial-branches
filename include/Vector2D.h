#pragma once
#include <cmath>

class Vector2D {
public:
    Vector2D(float x = 0, float y = 0) : m_x(x), m_y(y) {}

    float getX() const { return m_x; }
    float getY() const { return m_y; }
    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }

    float length() const { return std::sqrt(m_x * m_x + m_y * m_y); }
    float lengthSq() const { return m_x * m_x + m_y * m_y; } // 🆕 NEW: 제곱근 연산 최적화

    Vector2D operator+(const Vector2D& v2) const;
    Vector2D operator-(const Vector2D& v2) const;
    Vector2D operator*(float scalar) const;
    Vector2D operator/(float scalar) const;
    Vector2D operator-() const; // 🆕 NEW: 단항 마이너스 연산자

    Vector2D& operator*=(float scalar);
    Vector2D& operator/=(float scalar);

    // 🆕 NEW: 비교 연산자
    bool operator==(const Vector2D& other) const;
    bool operator!=(const Vector2D& other) const;

    void normalize();
    Vector2D normalized() const; // 🆕 NEW: 정규화된 벡터 반환

    friend Vector2D& operator+=(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator-=(Vector2D& v1, const Vector2D& v2);

private:
    float m_x;
    float m_y;
};