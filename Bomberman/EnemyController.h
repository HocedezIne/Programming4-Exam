#pragma once

#include "Singleton.h"
#include "GameObject.h"
#include "Observer.h"

namespace enemyController
{
	class EnemyController : public engine::Singleton<EnemyController>, public engine::Subject
	{
	public:
		void AddBalloomEnemy(int column, int row, glm::vec3 levelPosition);
		void KillEnemy(engine::GameObject* enemy);

	private:
		int m_Count{};
	};
}