#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "Collider.h"
#include <vector>

class CollisionManager {
public:
    static CollisionManager* Instance();
    static void Release();

    void addCollider(Collider* collider);
    void removeCollider(Collider* collider);
    void update();

    void setDebugDraw(bool debug) { m_debugDraw = debug; }
    void render(SDL_Renderer* renderer);

private:
    CollisionManager() = default;
    static CollisionManager* s_instance;

    std::vector<Collider*> m_colliders;
    bool m_debugDraw{ false };

    void checkCollisions();
    bool shouldCollide(Collider* a, Collider* b);
    void renderCollisionInfo(SDL_Renderer* renderer);
};

#endif // COLLISION_MANAGER_H