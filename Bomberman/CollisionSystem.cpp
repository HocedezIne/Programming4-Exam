#include "CollisionSystem.h"

#include <iostream>

#include "GameObject.h"

namespace collisionSystem
{
	std::vector<ColliderComponent*> colliders;
	CollisionHandler collisionHandler;

	void CollisionHandler::ResolveCollision(ColliderComponent* currCollider, ColliderComponent* otherCollider)
	{
		currCollider;
		otherCollider;

		switch (currCollider->m_CollisionType)
		{
		case CollisionType::Player:

			switch (otherCollider->m_CollisionType)
			{
			case CollisionType::Enemy:
				NotifyObservers(engine::Event::PlayerDied, nullptr, std::any{});
				break;

			case CollisionType::Wall:
				PlayerWallResolve(currCollider);
				break;

			case CollisionType::Block:
				PlayerBlockResolve(currCollider, otherCollider);

			default:
				break;
			}
				
			break;

		default:
			break;
		}
	}

	void CollisionHandler::PlayerBlockResolve(ColliderComponent* currCollider, ColliderComponent* otherCollider)
	{
		auto currPos = currCollider->GetOwner()->GetLocalPosition();
		auto otherPos = otherCollider->GetOwner()->GetLocalPosition();
		glm::vec3 movement{};
		
		if (currPos.x + currCollider->m_BoundingDimensions.x > otherPos.x ||
			otherPos.x + otherCollider->m_BoundingDimensions.x > currPos.x)
		{
			if (currPos.x < otherPos.x) movement.x = otherPos.x - currCollider->m_BoundingDimensions.x - currPos.x;
			else movement.x = otherPos.x + otherCollider->m_BoundingDimensions.x - currPos.x;
		}
		
		if (currPos.y + currCollider->m_BoundingDimensions.y > otherPos.y ||
			otherPos.y + otherCollider->m_BoundingDimensions.y > currPos.y)
		{
			if (currPos.y < otherPos.y) movement.y = otherPos.y - currCollider->m_BoundingDimensions.y - currPos.y;
			else movement.y = otherPos.y + otherCollider->m_BoundingDimensions.y - currPos.y;
		}
		
		currCollider->GetOwner()->SetLocalPosition(currPos + movement);
	}

	void CollisionHandler::PlayerWallResolve(ColliderComponent* currCollider)
	{
		auto currPos = currCollider->GetOwner()->GetLocalPosition();
		auto currDeltaPos = currPos - currCollider->GetOwner()->GetPreviousLocalPosition();
		currCollider->GetOwner()->SetLocalPosition(currPos - currDeltaPos);
	}
}