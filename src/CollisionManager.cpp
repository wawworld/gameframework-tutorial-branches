#include "CollisionManager.h"
#include "MonoBehaviour.h"
#include "Collider.h"
#include <algorithm>

CollisionManager* CollisionManager::s_instance = nullptr;

CollisionManager* CollisionManager::Instance() {
    if (!s_instance) {
        s_instance = new CollisionManager();
    }
    return s_instance;
}

void CollisionManager::Release() {
    if (s_instance) {
        delete s_instance;
        s_instance = nullptr;
    }
}

void CollisionManager::addCollider(Collider* collider)
{
    if (!collider) return;

    auto it = std::find(m_colliders.begin(), m_colliders.end(), collider);
    if (it == m_colliders.end()) {
        m_colliders.push_back(collider);
    }
}

void CollisionManager::removeCollider(Collider* collider)
{
    if (!collider) return;

    auto it = std::remove(m_colliders.begin(), m_colliders.end(), collider);
    if (it != m_colliders.end()) {
        m_colliders.erase(it, m_colliders.end());
    }
}

void CollisionManager::update() {
    checkCollisions();
}

void CollisionManager::render(SDL_Renderer* renderer) {
    if (!m_debugDraw) return;

    for (auto* collider : m_colliders) {
        collider->render(renderer);
    }

    renderCollisionInfo(renderer);
}

void CollisionManager::renderCollisionInfo(SDL_Renderer* renderer) {
    for (size_t i = 0; i < m_colliders.size(); i++) {
        for (size_t j = i + 1; j < m_colliders.size(); j++) {
            Collider* a = m_colliders[i];
            Collider* b = m_colliders[j];

            if (!shouldCollide(a, b)) continue;

            Collision collision;
            if (a->checkCollision(b, collision)) {
                // 충돌 지점 표시 (큰 노란색 점)
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_Rect collisionPoint = {
                    collision.point.getX() - 3,
                    collision.point.getY() - 3,
                    6, 6
                };
                SDL_RenderFillRect(renderer, &collisionPoint);

                // 법선 벡터 표시 (파란색 선)
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                Vector2D normalEnd = collision.point + (collision.normal * 30);
                SDL_RenderDrawLine(renderer,
                    collision.point.getX(), collision.point.getY(),
                    normalEnd.getX(), normalEnd.getY());
            }
        }
    }
}

void CollisionManager::checkCollisions() {
    for (size_t i = 0; i < m_colliders.size(); i++) {
        for (size_t j = i + 1; j < m_colliders.size(); j++) {
            Collider* a = m_colliders[i];
            Collider* b = m_colliders[j];

            if (!shouldCollide(a, b)) continue;

            Collision collisionA;
            if (a->checkCollision(b, collisionA)) {
                // 물리적 충돌 처리
                if (!a->isTrigger() && !b->isTrigger()) {
                    // 여기서 물리적 분리나 반발 처리
                }

                // Enter/Stay 이벤트 처리
                if (!a->isCollidingWith(b->getGameObject())) {
                    if (a->isTrigger() || b->isTrigger()) {
                        a->onTriggerEnter(collisionA);
                    }
                    else {
                        a->onCollisionEnter(collisionA);
                    }
                }
                else {
                    if (a->isTrigger() || b->isTrigger()) {
                        a->onTriggerStay(collisionA);
                    }
                    else {
                        a->onCollisionStay(collisionA);
                    }
                }

                // 반대 방향의 충돌 정보 생성
                Collision collisionB = collisionA;
                collisionB.normal = -collisionA.normal;
                collisionB.other = a->getGameObject();

                if (!b->isCollidingWith(a->getGameObject())) {
                    if (a->isTrigger() || b->isTrigger()) {
                        b->onTriggerEnter(collisionB);
                    }
                    else {
                        b->onCollisionEnter(collisionB);
                    }
                }
                else {
                    if (a->isTrigger() || b->isTrigger()) {
                        b->onTriggerStay(collisionB);
                    }
                    else {
                        b->onCollisionStay(collisionB);
                    }
                }

                // 충돌 상태 업데이트
                a->addCollision(b->getGameObject());
                b->addCollision(a->getGameObject());
            }
            else {
                // Exit 이벤트 처리
                if (a->isCollidingWith(b->getGameObject())) {
                    if (a->isTrigger() || b->isTrigger()) {
                        a->onTriggerExit({ b->getGameObject(), {}, {}, 0 });
                    }
                    else {
                        a->onCollisionExit({ b->getGameObject(), {}, {}, 0 });
                    }
                    a->removeCollision(b->getGameObject());
                }
                if (b->isCollidingWith(a->getGameObject())) {
                    if (a->isTrigger() || b->isTrigger()) {
                        b->onTriggerExit({ a->getGameObject(), {}, {}, 0 });
                    }
                    else {
                        b->onCollisionExit({ a->getGameObject(), {}, {}, 0 });
                    }
                    b->removeCollision(a->getGameObject());
                }
            }
        }
    }
}

//bool CollisionManager::shouldCollide(Collider* a, Collider* b) {
//    return static_cast<int>(a->getLayer()) & static_cast<int>(b->getLayer());
//}

// Collider에 충돌 허용 레이어가 있다고 가정
bool CollisionManager::shouldCollide(Collider* a, Collider* b) {
    return static_cast<int>(a->getLayer()) & static_cast<int>(b->getLayer());
}