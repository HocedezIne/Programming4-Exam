#pragma once

#include <string>

#include "Singleton.h"
#include "GameObject.h"
#include "Observer.h"
#include "Scene.h"

namespace enemyController
{
	class EnemyController : public engine::Singleton<EnemyController>, public engine::Subject
	{
	public:
		void AddBalloomEnemy(glm::vec3 pos, engine::GameObject* parent, engine::Scene& scene);
		void AddOnealEnemy(glm::vec3 pos, engine::GameObject* parent, engine::Scene& scene);
		void AddDollEnemy(glm::vec3 pos, engine::GameObject* parent, engine::Scene& scene);
		void AddMinvoEnemy(glm::vec3 pos, engine::GameObject* parent, engine::Scene& scene);

		void KillEnemy(engine::GameObject* enemy, engine::GameObject* player);

		void SetPlayers(const std::vector<engine::GameObject*> players) { m_Players = players; };

		int GetCount() const { return m_Count; };
		void ClearEnemies() { m_Count = 0; };

	private:
		template <typename StartState>
		void AddEnemy(glm::vec3 pos, engine::GameObject* parent, engine::Scene& scene, 
			const std::string& name, int points, float speed, float chasingDistance = 0.f);

		int m_Count{};
		std::vector<engine::GameObject*> m_Players;
	};
}