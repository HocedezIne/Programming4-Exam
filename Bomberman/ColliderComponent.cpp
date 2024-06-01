#include "ColliderComponent.h"

#include "GameObject.h"
#include "CollisionSystem.h"

ColliderComponent::ColliderComponent(engine::GameObject* owner, glm::vec2 bounds, bool isStatic, CollisionType cm) : engine::Component(owner),
	m_BoundingDimensions(bounds), m_IsStatic(isStatic), m_CollisionType(cm)
{
	collisionSystem::colliders.push_back(this);
}

ColliderComponent::~ColliderComponent()
{
	collisionSystem::colliders.erase(
		std::remove(collisionSystem::colliders.begin(), collisionSystem::colliders.end(), this), 
		collisionSystem::colliders.end());
}

void ColliderComponent::Update()
{
	for (const auto collider : collisionSystem::colliders)
	{
		if (m_IsStatic)
			continue;

		if (collider == this)
			continue;

		if (IsColliding(collider))
		{
			collisionSystem::collisionHandler.ResolveCollision(this, collider);
		}
	}
}

bool ColliderComponent::IsColliding(ColliderComponent* other)
{
	if (other->m_BoundingDimensions == glm::vec2{ 0,0 }) return false;

	auto Position = GetOwner()->GetWorldPosition();
	auto otherPosition = other->GetOwner()->GetWorldPosition();

	if (otherPosition.x + other->m_BoundingDimensions.x <= Position.x ||
		Position.x + m_BoundingDimensions.x <= otherPosition.x)
		return false;
	if (otherPosition.y + other->m_BoundingDimensions.y <= Position.y ||
		Position.y + m_BoundingDimensions.y <= otherPosition.y)
		return false;

	return true;
}

//void ColliderComponent::ResolveCollision(ColliderComponent* other)
//{
//	auto Position = GetOwner()->GetWorldPosition();
//	auto otherPosition = other->GetOwner()->GetWorldPosition();
//
//	switch (m_CollisionMode)
//	{
//	case CollisionMode::SoftBlock:
//		switch (other->m_CollisionMode) 
//		{
//		case CollisionMode::Dynamic:
//			glm::vec3 movement{};
//
//			if (otherPosition.x + other->m_BoundingDimensions.x > Position.x ||
//				Position.x + m_BoundingDimensions.x > otherPosition.x)
//			{
//				if (otherPosition.x < Position.x) movement.x = Position.x - other->m_BoundingDimensions.x - otherPosition.x;
//				else movement.x = Position.x + m_BoundingDimensions.x - otherPosition.x;
//			}
//
//			if (otherPosition.y + other->m_BoundingDimensions.y > Position.y ||
//				Position.y + m_BoundingDimensions.y > otherPosition.y)
//			{
//				if (otherPosition.y < Position.y) movement.y = Position.y - other->m_BoundingDimensions.y - otherPosition.y;
//				else movement.y = Position.y + m_BoundingDimensions.y - otherPosition.y;
//			}
//
//			other->GetOwner()->SetLocalPosition(otherPosition + movement);
//			break;
//
//		default:
//			std::cout << "Collision type not implemented\n";
//			break;
//		}
//		break;
//	case CollisionMode::HardBlock:
//		switch (other->m_CollisionMode)
//		{
//		case CollisionMode::Dynamic:
//			auto otherDeltaPos = otherPosition - other->GetOwner()->GetPreviousLocalPosition();
//			other->GetOwner()->SetLocalPosition(otherPosition - otherDeltaPos);
//			break;
//		case CollisionMode::HardBlock:
//			break;
//		default:
//			std::cout << "Collision type not implemented\n";
//			break;
//		}
//		break;
//	default:
//		std::cout << "Collision type not implemented\n";
//		break;
//	}
//}