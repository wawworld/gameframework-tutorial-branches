#include "CollisionManager.h"
#include <algorithm>
#include <iostream>

CollisionManager* CollisionManager::s_pInstance = nullptr;

void CollisionManager::addGameObject(GameObject* pGameObject) {
    m_gameObjects.push_back(pGameObject);
}

void CollisionManager::removeGameObject(GameObject* pGameObject) {
    m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), pGameObject), m_gameObjects.end());
}

void CollisionManager::clearGameObjects() {
    m_gameObjects.clear();
}

bool CollisionManager::checkCollision(GameObject* p1, GameObject* p2) {
    int leftA = p1->getPosition().getX();
    int rightA = p1->getPosition().getX() + p1->getWidth();
    int topA = p1->getPosition().getY();
    int bottomA = p1->getPosition().getY() + p1->getHeight();

    int leftB = p2->getPosition().getX();
    int rightB = p2->getPosition().getX() + p2->getWidth();
    int topB = p2->getPosition().getY();
    int bottomB = p2->getPosition().getY() + p2->getHeight();

    // 분리 축 이론 적용
    if (bottomA <= topB) return false;
    if (topA >= bottomB) return false;
    if (rightA <= leftB) return false;
    if (leftA >= rightB) return false;

    return true;
}

void CollisionManager::update() {
    for (size_t i = 0; i < m_gameObjects.size(); ++i) {
        for (size_t j = i + 1; j < m_gameObjects.size(); ++j) {
            if (checkCollision(m_gameObjects[i], m_gameObjects[j])) {
                m_gameObjects[i]->onCollision(m_gameObjects[j]);
                m_gameObjects[j]->onCollision(m_gameObjects[i]);
            }
        }
    }
}