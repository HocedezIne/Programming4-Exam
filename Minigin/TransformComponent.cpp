#include "TransformComponent.h"

namespace engine
{
	TransformComponent::TransformComponent(GameObject*  owner, float x, float y, float z) : Component(owner)
	{
		SetPosition(x, y, z);
	}

	TransformComponent::TransformComponent(GameObject* owner, glm::vec3 pos) : Component(owner)
	{
		SetPosition(pos);
	}

	void TransformComponent::SetPosition(const glm::vec3 pos)
	{
		m_Position = pos;
	}
	void TransformComponent::SetPosition(const float x, const float y, const float z)
	{
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
	}
}