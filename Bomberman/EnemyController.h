#pragma once

#include "Singleton.h"
#include "GameObject.h"
#include "Observer.h"

namespace enemyController
{
	class EnemyController : public engine::Singleton<EnemyController>, public engine::Subject
	{
	public:
		void AddBalloomEnemy(int column, int row);
		void KillEnemy(engine::GameObject* enemy);

		int GetCount() const { return m_Count; };

	private:
		int m_Count{};
	};
}