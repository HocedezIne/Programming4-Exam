#include "ChasingBehaviour.h"

EnemyStateInterface* WalkTillChasing::HandleInput()
{
	for (int idx{}; idx < std::ssize(m_Players); ++idx)
	{
		if (glm::distance(m_Players[idx]->GetLocalPosition(), m_EnemyObject->GetLocalPosition()) < m_ChasingDistance)
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
		if (glm::distance(m_Players[idx]->GetLocalPosition(), m_EnemyObject->GetLocalPosition()) < m_ChasingDistance)
			return new WalkTillChasing(m_Speed, m_EnemyObject, m_Players, m_ChasingDistance);
	}

	return nullptr;
}