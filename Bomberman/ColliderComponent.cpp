#include "ColliderComponent.h"

#include "GameObject.h"
#include "CollisionSystem.h"

ColliderComponent::ColliderComponent(engine::GameObject* owner, glm::vec2 bounds, bool isStatic, CollisionType cm, glm::vec3 positionOffset) : engine::Component(owner),
	m_BoundingDimensions(bounds), m_IsStatic(isStatic), m_CollisionType(cm)
{
	m_Position += positionOffset;
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

	auto Position = GetOwner()->GetWorldPosition() + m_Position;
	auto otherPosition = other->GetOwner()->GetWorldPosition() + other->m_Position;

	if (otherPosition.x + other->m_BoundingDimensions.x <= Position.x ||
		Position.x + m_BoundingDimensions.x <= otherPosition.x)
		return false;
	if (otherPosition.y + other->m_BoundingDimensions.y <= Position.y ||
		Position.y + m_BoundingDimensions.y <= otherPosition.y)
		return false;

	return true;
}