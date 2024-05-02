#pragma once

#include <glm/glm.hpp>

#include "Component.h"

namespace engine
{
	class CollidorComponent final : public Component
	{
	public:
		CollidorComponent(GameObject* owner, glm::vec2 bounds);
		~CollidorComponent() = default;
		CollidorComponent(const CollidorComponent& other) = delete;
		CollidorComponent(CollidorComponent&& other) = delete;
		CollidorComponent& operator=(const CollidorComponent& other) = delete;
		CollidorComponent& operator=(CollidorComponent&& other) = delete;

		void CheckCollision(CollidorComponent* other);

	private:
		glm::vec2 m_BoundingDimensions{ 0,0 };
	};
}