#include "CollidorComponent.h"

#include "GameObject.h"

namespace engine
{
	CollidorComponent::CollidorComponent(GameObject* owner, glm::vec2 bounds) : Component(owner), m_BoundingDimensions(bounds)
	{}

	void CollidorComponent::CheckCollision(CollidorComponent* other)
	{
		if (other->m_BoundingDimensions == glm::vec2{ 0,0 }) return;

		auto otherGameObject = other->GetOwner();
		if (otherGameObject->GetWorldPosition().x + other->m_BoundingDimensions.x < GetOwner()->GetWorldPosition().x ||
			GetOwner()->GetWorldPosition().x + m_BoundingDimensions.x < otherGameObject->GetWorldPosition().x)
			return;
		if (otherGameObject->GetWorldPosition().y + other->m_BoundingDimensions.y < GetOwner()->GetWorldPosition().y ||
			GetOwner()->GetWorldPosition().y + m_BoundingDimensions.y < otherGameObject->GetWorldPosition().y)
			return;

		// handle collision
	}
}