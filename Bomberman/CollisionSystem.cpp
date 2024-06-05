#include "CollisionSystem.h"

#include <iostream>

#include "ServiceLocator.h"
#include "GameObject.h"
#include "DataComponent.h"
#include "EnemyController.h"

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
			case CollisionType::Explosion:
				NotifyObservers(engine::Event::PlayerDied, nullptr, std::any{});
				break;

			case CollisionType::Wall:
				PlayerWallResolve(currCollider);
				break;

			case CollisionType::Block:
			case CollisionType::Destructable:
				PlayerBlockResolve(currCollider, otherCollider);
				break;

			case CollisionType::Door:
				NotifyObservers(engine::Event::PlayerOnExit, otherCollider->GetOwner(), std::any{});

			default:
				break;
			}
				
			break;

		case CollisionType::Enemy:

			switch (otherCollider->m_CollisionType)
			{
			case CollisionType::Explosion:
				enemyController::EnemyController::GetInstance().KillEnemy(currCollider->GetOwner());
				break;

			case CollisionType::Wall:
			case CollisionType::Block:
				NotifyObservers(engine::Event::EnemyWalkedIntoWall, currCollider->GetOwner(), std::any{});
				break;
			}

			break;

		case CollisionType::Explosion:

			switch (otherCollider->m_CollisionType)
			{
			case CollisionType::Wall:
			case CollisionType::Block:
				currCollider->GetOwner()->MarkDeletion();
				break;

			case CollisionType::Destructable:
				otherCollider->GetOwner()->MarkDeletion();
				break;

			case CollisionType::Door:
				bool blocked = std::any_cast<bool>(otherCollider->GetOwner()->GetComponent<DataComponent>()->GetData("BLOCKED"));
				if (enemyController::EnemyController::GetInstance().GetCount() == 0 && blocked)
				{
					engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/BombermanDoorUnlock.wav", false);
				}
				otherCollider->GetOwner()->GetComponent<DataComponent>()->UpdateData("BLOCKED", false);
				break;
			}

			break;

		default:
			break;
		}
	}

	void CollisionHandler::PlayerBlockResolve(ColliderComponent* currCollider, ColliderComponent* otherCollider)
	{
		auto currPos = currCollider->GetOwner()->GetWorldPosition();
		auto otherPos = otherCollider->GetOwner()->GetWorldPosition();
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
		
		auto localPos = currCollider->GetOwner()->GetLocalPosition();
		currCollider->GetOwner()->SetLocalPosition(localPos + movement);
	}

	void CollisionHandler::PlayerWallResolve(ColliderComponent* currCollider)
	{
		auto currPos = currCollider->GetOwner()->GetLocalPosition();
		auto currDeltaPos = currPos - currCollider->GetOwner()->GetPreviousLocalPosition();
		currCollider->GetOwner()->SetLocalPosition(currPos - currDeltaPos);
	}
}