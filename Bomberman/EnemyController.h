#pragma once

#include <string>

#include "Singleton.h"
#include "GameObject.h"
#include "Observer.h"

namespace enemyController
{
	class EnemyController : public engine::Singleton<EnemyController>, public engine::Subject
	{
	public:
		void AddBalloomEnemy(int column, int row);
		void AddOnealEnemy(int column, int row);
		void AddDollEnemy(int column, int row);
		void AddMinvoEnemy(int column, int row);

		void KillEnemy(engine::GameObject* enemy);

		void SetPlayers(const std::vector<engine::GameObject*> players) { m_Players = players; };

		int GetCount() const { return m_Count; };

	private:
		template <typename StartState>
		void AddEnemy(int column, int row, const std::string& name, int points,
			float speed, float chasingDistance = 0.f);

		int m_Count{};
		std::vector<engine::GameObject*> m_Players;
	};
}