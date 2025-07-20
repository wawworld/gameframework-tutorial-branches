#include "Collider.h"
#include "GameObject.h"
#include "CollisionManager.h"
#include "MonoBehaviour.h"
#include <algorithm>
#include <iostream>
#include <cmath>

void Collider::init()
{
    m_transform = getGameObject()->getComponent<Transform>();
    if (!m_transform) {
        std::cerr << "Collider requires Transform component" << std::endl;
        return;
    }

    CollisionManager::Instance()->addCollider(this);
    updateBounds();
}

void Collider::destroy()
{
    CollisionManager::Instance()->removeCollider(this);
}

void Collider::fixedUpdate()
{
    checkTransformChange();
}

void Collider::onCollisionEnter(const Collision& collision) {
    auto monoBehaviour = getGameObject()->getComponent<MonoBehaviour>();
    if (monoBehaviour) {
        monoBehaviour->onCollisionEnter(collision);
    }
}

void Collider::onCollisionStay(const Collision& collision) {
    auto monoBehaviour = getGameObject()->getComponent<MonoBehaviour>();
    if (monoBehaviour) {
        monoBehaviour->onCollisionStay(collision);
    }
}

void Collider::onCollisionExit(const Collision& collision) {
    auto monoBehaviour = getGameObject()->getComponent<MonoBehaviour>();
    if (monoBehaviour) {
        monoBehaviour->onCollisionExit(collision);
    }
}

void Collider::onTriggerEnter(const Collision& collision) {
    auto monoBehaviour = getGameObject()->getComponent<MonoBehaviour>();
    if (monoBehaviour) {
        monoBehaviour->onTriggerEnter(collision);
    }
}

void Collider::onTriggerStay(const Collision& collision) {
    auto monoBehaviour = getGameObject()->getComponent<MonoBehaviour>();
    if (monoBehaviour) {
        monoBehaviour->onTriggerStay(collision);
    }
}

void Collider::onTriggerExit(const Collision& collision) {
    auto monoBehaviour = getGameObject()->getComponent<MonoBehaviour>();
    if (monoBehaviour) {
        monoBehaviour->onTriggerExit(collision);
    }
}

void Collider::checkTransformChange()
{
    if (m_transform) {
        Vector2D currentPosition = m_transform->getPosition();
        Vector2D currentScale = m_transform->getScale();
        float currentRotation = m_transform->getRotation();

        if (currentPosition != m_lastPosition ||
            currentScale != m_lastScale ||
            currentRotation != m_lastRotation) {
            updateBounds();
            m_lastPosition = currentPosition;
            m_lastScale = currentScale;
            m_lastRotation = currentRotation;
        }
    }
}

bool Collider::isCollidingWith(GameObject* other) const {
    return m_currentCollisions.find(other) != m_currentCollisions.end();
}

void Collider::addCollision(GameObject* other) {
    m_currentCollisions.insert(other);
}

void Collider::removeCollision(GameObject* other) {
    m_currentCollisions.erase(other);
}

// BoxCollider 구현
void BoxCollider::init()
{
    Collider::init();
}

bool BoxCollider::checkCollision(Collider* other, Collision& collision)
{
    // 다른 BoxCollider와의 충돌 검사
    auto otherBox = dynamic_cast<BoxCollider*>(other);
    if (otherBox) {
        const SDL_Rect& a = m_bounds;
        const SDL_Rect& b = otherBox->getBounds();

        // AABB 충돌 검사
        if (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y)
        {
            collision.other = other->getGameObject();

            // 충돌 지점 계산
            collision.point.setX((a.x + a.w / 2 + b.x + b.w / 2) / 2);
            collision.point.setY((a.y + a.h / 2 + b.y + b.h / 2) / 2);

            // 침투 벡터 계산
            float dx = (a.x + a.w / 2) - (b.x + b.w / 2);
            float dy = (a.y + a.h / 2) - (b.y + b.h / 2);

            // 침투 깊이 계산
            float px = (a.w + b.w) / 2 - std::abs(dx);
            float py = (a.h + b.h) / 2 - std::abs(dy);

            // 가장 얕은 침투 방향으로 normal 설정
            if (px < py) {
                collision.normal = Vector2D(dx > 0 ? 1 : -1, 0);
                collision.penetration = px;
            }
            else {
                collision.normal = Vector2D(0, dy > 0 ? 1 : -1);
                collision.penetration = py;
            }

            return true;
        }
    }

    // CircleCollider와의 충돌 검사
    auto otherCircle = dynamic_cast<CircleCollider*>(other);
    if (otherCircle) {
        Vector2D boxCenter(m_bounds.x + m_bounds.w / 2, m_bounds.y + m_bounds.h / 2);
        Vector2D circleCenter = other->getGameObject()->getTransform()->getPosition();

        Vector2D closest = circleCenter;
        if (closest.getX() < m_bounds.x) closest.setX(m_bounds.x);
        if (closest.getX() > m_bounds.x + m_bounds.w) closest.setX(m_bounds.x + m_bounds.w);
        if (closest.getY() < m_bounds.y) closest.setY(m_bounds.y);
        if (closest.getY() > m_bounds.y + m_bounds.h) closest.setY(m_bounds.y + m_bounds.h);

        Vector2D distance = circleCenter - closest;
        float distanceSquared = distance.lengthSq();
        float radiusSquared = otherCircle->getRadius() * otherCircle->getRadius();

        if (distanceSquared < radiusSquared) {
            float dist = std::sqrt(distanceSquared);
            collision.other = other->getGameObject();
            collision.point = closest;
            collision.normal = distance / dist;
            collision.penetration = otherCircle->getRadius() - dist;
            return true;
        }
    }

    return false;
}

void BoxCollider::updateBounds()
{
    if (!m_transform) return;

    Vector2D pos = m_transform->getPosition();
    Vector2D scale = m_transform->getScale();

    m_bounds.x = pos.getX() + m_offset.getX();
    m_bounds.y = pos.getY() + m_offset.getY();
    m_bounds.w = m_size.getX() * scale.getX();
    m_bounds.h = m_size.getY() * scale.getY();
}

void BoxCollider::render(SDL_Renderer* renderer)
{
    if (!m_transform) return;

    // 충돌 상태에 따른 색상 변경
    if (m_currentCollisions.empty()) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // 녹색: 정상
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // 빨간색: 충돌 중
    }

    SDL_Rect drawRect = m_bounds;
    SDL_RenderDrawRect(renderer, &drawRect);

    // 충돌체 중심점 표시
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // 노란색 점
    SDL_Rect centerPoint = {
        m_bounds.x + m_bounds.w / 2 - 1,
        m_bounds.y + m_bounds.h / 2 - 1,
        2, 2
    };
    SDL_RenderFillRect(renderer, &centerPoint);
}

// CircleCollider 구현
void CircleCollider::init()
{
    Collider::init();
}

bool CircleCollider::checkCollision(Collider* other, Collision& collision)
{
    // 다른 CircleCollider와의 충돌 검사
    auto otherCircle = dynamic_cast<CircleCollider*>(other);
    if (otherCircle) {
        Vector2D myCenter = m_transform->getPosition();
        Vector2D otherCenter = other->getGameObject()->getTransform()->getPosition();

        Vector2D distance = otherCenter - myCenter;
        float distanceSquared = distance.lengthSq();
        float radiusSum = m_radius + otherCircle->getRadius();

        if (distanceSquared < radiusSum * radiusSum) {
            float dist = std::sqrt(distanceSquared);
            collision.other = other->getGameObject();
            collision.point = myCenter + (distance * 0.5f);
            collision.normal = distance / dist;
            collision.penetration = radiusSum - dist;
            return true;
        }
    }

    // BoxCollider와의 충돌은 BoxCollider에서 처리됨
    return false;
}

void CircleCollider::updateBounds()
{
    if (!m_transform) return;

    Vector2D pos = m_transform->getPosition();
    Vector2D scale = m_transform->getScale();

    float scaledRadius = m_radius * std::max(scale.getX(), scale.getY());

    m_bounds.x = pos.getX() + m_offset.getX() - scaledRadius;
    m_bounds.y = pos.getY() + m_offset.getY() - scaledRadius;
    m_bounds.w = scaledRadius * 2;
    m_bounds.h = scaledRadius * 2;
}

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void CircleCollider::render(SDL_Renderer* renderer)
{
    if (!m_transform) return;

    // 충돌 상태에 따른 색상 변경
    if (m_currentCollisions.empty()) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // 녹색: 정상
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // 빨간색: 충돌 중
    }

    float centerX = m_bounds.x + m_bounds.w / 2;
    float centerY = m_bounds.y + m_bounds.h / 2;
    float radius = m_bounds.w / 2;

    DrawCircle(renderer,
        static_cast<int>(centerX),
        static_cast<int>(centerY),
        static_cast<int>(radius));

    // 원의 중심점 표시
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // 노란색 점
    SDL_Rect centerPoint = { centerX - 1, centerY - 1, 2, 2 };
    SDL_RenderFillRect(renderer, &centerPoint);
}