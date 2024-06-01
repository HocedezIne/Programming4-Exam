#pragma once
#include <vector>

#include "ColliderComponent.h"
#include "Observer.h"

namespace collisionSystem
{
	extern std::vector<ColliderComponent*> colliders;
	extern CollisionHandler collisionHandler;

	class CollisionHandler : public engine::Subject
	{
	public:
		CollisionHandler() = default;
		~CollisionHandler() = default;

		void ResolveCollision(ColliderComponent* currCollider, ColliderComponent* otherCollider);
	
	private:
		void PlayerBlockResolve(ColliderComponent* currCollider, ColliderComponent* otherCollider);
		void PlayerWallResolve(ColliderComponent* currCollider);
	};
}