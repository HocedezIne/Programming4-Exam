#include "ColliderComponent.h"

#include "GameObject.h"
#include <iostream>

namespace engine
{
	ColliderComponent::ColliderComponent(GameObject* owner, glm::vec2 bounds, CollisionMode cm) : Component(owner), 
		m_BoundingDimensions(bounds), m_CollisionMode(cm)
	{}

	void ColliderComponent::CheckCollision(ColliderComponent* other)
	{
		if (other->m_BoundingDimensions == glm::vec2{ 0,0 }) return;

		auto Position = GetOwner()->GetWorldPosition();
		auto otherPosition = other->GetOwner()->GetWorldPosition();

		if (otherPosition.x + other->m_BoundingDimensions.x <= Position.x ||
			Position.x + m_BoundingDimensions.x <= otherPosition.x)
			return;
		if (otherPosition.y + other->m_BoundingDimensions.y <= Position.y ||
			Position.y + m_BoundingDimensions.y <= otherPosition.y)
			return;

		ResolveCollision(other);
	}

	void ColliderComponent::ResolveCollision(ColliderComponent* other)
	{
		auto Position = GetOwner()->GetWorldPosition();
		auto otherPosition = other->GetOwner()->GetWorldPosition();

		// handle collision
		switch (m_CollisionMode)
		{
		case CollisionMode::Overlap:
			switch (other->m_CollisionMode)
			{
			case CollisionMode::Overlap:
				std::cout << "Overlap + overlap\n";
				break;
			case CollisionMode::Dynamic:
				std::cout << "Overlap + dynamic\n";
				break;
			case CollisionMode::Block:
				std::cout << "Overlap + block\n";
				break;
			}
			break;
		case CollisionMode::Dynamic:
			switch (other->m_CollisionMode)
			{
			case CollisionMode::Overlap:
				std::cout << "Dynamic + overlap\n";
				break;
			case CollisionMode::Dynamic:
				std::cout << "Dynamic + dynamic\n";
				break;
			case CollisionMode::Block:
				//glm::vec3 newPos{};
				//newPos.x = (Position.x < otherPosition.x) ? otherPosition.x - m_BoundingDimensions.x : otherPosition.x + other->m_BoundingDimensions.x;
				//newPos.y = (Position.y < otherPosition.y) ? otherPosition.y - m_BoundingDimensions.y : otherPosition.y + other->m_BoundingDimensions.y;
				//GetOwner()->SetLocalPosition(newPos);
				break;
			}
			break;
		case CollisionMode::Block:
			switch (other->m_CollisionMode)
			{
			case CollisionMode::Overlap:
				std::cout << "Block + overlap\n";
				break;
			case CollisionMode::Dynamic:
				glm::vec3 movement{};

				if (otherPosition.x + other->m_BoundingDimensions.x > Position.x ||
					Position.x + m_BoundingDimensions.x > otherPosition.x)
				{
					if (otherPosition.x < Position.x) movement.x = Position.x - other->m_BoundingDimensions.x - otherPosition.x;
					else movement.x = Position.x + m_BoundingDimensions.x - otherPosition.x;
				}

				if (otherPosition.y + other->m_BoundingDimensions.y > Position.y ||
					Position.y + m_BoundingDimensions.y > otherPosition.y)
				{
					if (otherPosition.y < Position.y) movement.y = Position.y - other->m_BoundingDimensions.y - otherPosition.y;
					else movement.y = Position.y + m_BoundingDimensions.y - otherPosition.y;
				}

				other->GetOwner()->SetLocalPosition(otherPosition + movement);
				break;
			}
			break;
		}
	}
}