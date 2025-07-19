#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <vector>
#include "GameObject.h"

class CollisionManager {
public:
    static CollisionManager* Instance() {
        if (s_pInstance == nullptr) {
            s_pInstance = new CollisionManager();
        }
        return s_pInstance;
    }

    void addGameObject(GameObject* pGameObject);
    void removeGameObject(GameObject* pGameObject);
    void clearGameObjects();
    bool checkCollision(GameObject* p1, GameObject* p2);
    void update();

private:
    CollisionManager() {}
    ~CollisionManager() {}

    static CollisionManager* s_pInstance;
    std::vector<GameObject*> m_gameObjects;
};

#endif // COLLISIONMANAGER_H