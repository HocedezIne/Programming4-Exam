#pragma once

#include <glm/glm.hpp>

#include "Component.h"

namespace engine
{
	enum class CollisionMode
	{
		Overlap,
		Dynamic,
		Block
	};

	class ColliderComponent final : public Component
	{
	public:
		ColliderComponent(GameObject* owner, glm::vec2 bounds, CollisionMode cm);
		~ColliderComponent() = default;
		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) = delete;
		ColliderComponent& operator=(const ColliderComponent& other) = delete;
		ColliderComponent& operator=(ColliderComponent&& other) = delete;

		void CheckCollision(ColliderComponent* other);

	private:
		glm::vec2 m_BoundingDimensions{ 0,0 };
		const CollisionMode m_CollisionMode;
	};
}