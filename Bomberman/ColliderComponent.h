#pragma once

#include <glm/glm.hpp>

#include "Component.h"

enum class CollisionType
{
	Player,
	Enemy,
	Wall,
	Block,
	Explosion,
	PowerUp
};

namespace collisionSystem {
	class CollisionHandler;
}

class ColliderComponent final : public engine::Component, public engine::IUpdatable
{
public:
	ColliderComponent(engine::GameObject* owner, glm::vec2 bounds, bool isStatic, CollisionType cm);
	~ColliderComponent();
	ColliderComponent(const ColliderComponent& other) = delete;
	ColliderComponent(ColliderComponent&& other) = delete;
	ColliderComponent& operator=(const ColliderComponent& other) = delete;
	ColliderComponent& operator=(ColliderComponent&& other) = delete;

	virtual void Update() override;

	bool IsColliding(ColliderComponent* other);

private:
	glm::vec2 m_BoundingDimensions{ 0,0 };
	const bool m_IsStatic;
	const CollisionType m_CollisionType;

	friend class collisionSystem::CollisionHandler;
};