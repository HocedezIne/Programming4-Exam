#include "CollisionSystem.h"

#include <iostream>

#include "SceneManager.h"
#include "ServiceLocator.h"
#include "EnemyController.h"

#include "GameObject.h"

#include "DataComponent.h"

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
			case CollisionType::PlayableEnemy:
			case CollisionType::Enemy:
			case CollisionType::Explosion:
				NotifyObservers(engine::Event::PlayerDied, currCollider->GetOwner(), std::any{});
				break;

			case CollisionType::Wall:
				WallResolve(currCollider);
				break;

			case CollisionType::Block:
			case CollisionType::Destructable:
				BlockResolve(currCollider, otherCollider);
				break;

			case CollisionType::Door:
				NotifyObservers(engine::Event::PlayerOnExit, otherCollider->GetOwner(), std::any{});
				break;

			case CollisionType::PowerUp:
			{
				bool blocked = std::any_cast<bool>(otherCollider->GetOwner()->GetComponent<DataComponent>()->GetData("BLOCKED"));
				if (!blocked)
				{
					NotifyObservers(engine::Event::PowerUpCollected, otherCollider->GetOwner(), std::any{});
					otherCollider->GetOwner()->MarkDeletion();
				}
				break;

			}

			default:
				break;
			}
				
			break;

		case CollisionType::Enemy:

			switch (otherCollider->m_CollisionType)
			{
			case CollisionType::Explosion:
			{
				auto player = std::any_cast<engine::GameObject*>(otherCollider->GetOwner()->GetComponent<DataComponent>()->GetData("PLAYER"));
				enemyController::EnemyController::GetInstance().KillEnemy(currCollider->GetOwner(), player);
				break;
			}
			case CollisionType::Wall:
			case CollisionType::Block:
			case CollisionType::Bomb:
			case CollisionType::Destructable:
				NotifyObservers(engine::Event::EnemyWalkedIntoWall, currCollider->GetOwner(), std::any{});
				break;
			}

			break;

		case CollisionType::PlayableEnemy:

			switch (otherCollider->m_CollisionType)
			{
			case CollisionType::Explosion:
				NotifyObservers(engine::Event::PlayableEnemyDied, currCollider->GetOwner(), std::any{});
				break;

			case CollisionType::Wall:
				WallResolve(currCollider);
				break;

			case CollisionType::Block:
			case CollisionType::Bomb:
			case CollisionType::Destructable:
				BlockResolve(currCollider, otherCollider);
				break;

			default:
				break;
			}
			break;

		case CollisionType::Explosion:

			switch (otherCollider->m_CollisionType)
			{
			case CollisionType::Wall:
			case CollisionType::Block:
			{
				auto identifier = engine::sceneManager::currentScenes[1]->GetObjectIdentifier(currCollider->GetOwner());
				auto toDelete = currCollider->GetOwner();
				do
				{
					toDelete->MarkDeletion();
					identifier = identifier.substr(0, identifier.find('.')) + "." + (++identifier.back());
					toDelete = engine::sceneManager::currentScenes[1]->GetObject(identifier);
				} while ( toDelete );

				break;
			}

			case CollisionType::Destructable:
			{
				if (!otherCollider->GetOwner()->IsMarkedForDeletion())
				{
					otherCollider->GetOwner()->MarkDeletion();

					bool firstSkip{ true };
					auto identifier = engine::sceneManager::currentScenes[1]->GetObjectIdentifier(currCollider->GetOwner());
					auto toDelete = currCollider->GetOwner();
					do
					{
						if (!firstSkip)toDelete->MarkDeletion();
						else firstSkip = false;
						identifier = identifier.substr(0, identifier.find('.')) + "." + (++identifier.back());
						toDelete = engine::sceneManager::currentScenes[1]->GetObject(identifier);
					} while (toDelete);
				}
				break;
			}

			case CollisionType::Door:
			{
				bool blocked = std::any_cast<bool>(otherCollider->GetOwner()->GetComponent<DataComponent>()->GetData("BLOCKED"));
				if (enemyController::EnemyController::GetInstance().GetCount() == 0 && blocked)
				{
					engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/BombermanDoorUnlock.wav", false);
				}
				otherCollider->GetOwner()->GetComponent<DataComponent>()->UpdateData("BLOCKED", false);
				break;
			}

			case CollisionType::PowerUp:
				otherCollider->GetOwner()->GetComponent<DataComponent>()->UpdateData("BLOCKED", false);
				break;

			}
			break;

		default:
			break;
		}
	}

	void CollisionHandler::BlockResolve(ColliderComponent* currCollider, ColliderComponent* otherCollider)
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

	void CollisionHandler::WallResolve(ColliderComponent* currCollider)
	{
		auto currPos = currCollider->GetOwner()->GetLocalPosition();
		auto currDeltaPos = currPos - currCollider->GetOwner()->GetPreviousLocalPosition();
		currCollider->GetOwner()->SetLocalPosition(currPos - currDeltaPos);
	}
}