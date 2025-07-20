#ifndef COLLIDER_H
#define COLLIDER_H


#include "Component.h"
#include "Transform.h"
#include <SDL2/SDL.h>
#include <vector>
#include <unordered_set>


// 🆕 NEW: 충돌 정보를 담는 구조체
struct Collision {
	GameObject* other; // 충돌한 다른 게임오브젝트
	Vector2D point; // 충돌 지점
	Vector2D normal; // 충돌 방향
	float penetration; // 충돌 침투 깊이
};


// 🆕 NEW: 충돌체 타입 구분
enum class ColliderType {
	Box,
	Circle
};


// 🆕 NEW: 충돌 레이어 (비트마스크)
enum class CollisionLayer {
	None = 0,
	Default = 1,
	Player = 1 << 1,
	Enemy = 1 << 2,
	Projectile = 1 << 3,
	Ground = 1 << 4
};


// 기본 Collider 클래스
class Collider : public Component {
public:
	void init() override;
	void destroy() override;
	void fixedUpdate() override;


	// 충돌 체크
	virtual bool checkCollision(Collider* other, Collision& collision) = 0;


	// 충돌 이벤트 (MonoBehaviour로 전달)
	virtual void onCollisionEnter(const Collision& collision);
	virtual void onCollisionStay(const Collision& collision);
	virtual void onCollisionExit(const Collision& collision);


	// 트리거 이벤트 (MonoBehaviour로 전달)
	virtual void onTriggerEnter(const Collision& collision);
	virtual void onTriggerStay(const Collision& collision);
	virtual void onTriggerExit(const Collision& collision);


	// 설정
	void setLayer(CollisionLayer layer) { m_layer = layer; }
	void setTrigger(bool isTrigger) { m_isTrigger = isTrigger; }


	// 게터
	CollisionLayer getLayer() const { return m_layer; }
	bool isTrigger() const { return m_isTrigger; }
	const SDL_Rect& getBounds() const { return m_bounds; }


	// 충돌 상태 관리
	bool isCollidingWith(GameObject* other) const;
	void addCollision(GameObject* other);
	void removeCollision(GameObject* other);


protected:
	Transform* m_transform{ nullptr };
	SDL_Rect m_bounds{};
	CollisionLayer m_layer{ CollisionLayer::Default };
	bool m_isTrigger{ false };
	std::unordered_set<GameObject*> m_currentCollisions;


	virtual void updateBounds() = 0;
	void checkTransformChange();


private:
	Vector2D m_lastPosition;
	Vector2D m_lastScale;
	float m_lastRotation;
};


// BoxCollider 구현
class BoxCollider : public Collider {
public:
	void init() override;
	bool checkCollision(Collider* other, Collision& collision) override;
	void render(SDL_Renderer* renderer) override;


	void setSize(const Vector2D& size) { m_size = size; updateBounds(); }
	void setOffset(const Vector2D& offset) { m_offset = offset; updateBounds(); }


protected:
	void updateBounds() override;


private:
	Vector2D m_size{ 32, 32 };
	Vector2D m_offset{ 0, 0 };
};


// CircleCollider 구현
class CircleCollider : public Collider {
public:
	void init() override;
	bool checkCollision(Collider* other, Collision& collision) override;
	void render(SDL_Renderer* renderer) override;


	void setRadius(float radius) { m_radius = radius; updateBounds(); }
	float getRadius() const { return m_radius; }
	void setOffset(const Vector2D& offset) { m_offset = offset; updateBounds(); }


protected:
	void updateBounds() override;


private:
	float m_radius{ 16.0f };
	Vector2D m_offset{ 0, 0 };
};


#endif // COLLIDER_H