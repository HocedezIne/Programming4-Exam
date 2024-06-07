#include "ChasingBehaviour.h"

EnemyStateInterface* WalkTillChasing::HandleInput()
{
	for (int idx{}; idx < std::ssize(m_Players); ++idx)
	{
		auto distance = abs(m_Players[idx]->GetLocalPosition() - m_EnemyObject->GetLocalPosition());
		if (glm::length(distance) < 5 * 16.f)
		{
			return new Chasing(m_Speed, m_EnemyObject, m_Players, m_ChasingDistance, idx);
		}
	}

	return nullptr;
}

EnemyStateInterface* Chasing::HandleInput()
{
	for (int idx{}; idx < std::ssize(m_Players); ++idx)
	{
		auto distance = abs(m_Players[idx]->GetLocalPosition() - m_EnemyObject->GetLocalPosition());
		if (glm::length(distance) > 5 * 16.f)
			return new WalkTillChasing(m_Speed, m_EnemyObject, m_Players, m_ChasingDistance);
	}

	return nullptr;
}