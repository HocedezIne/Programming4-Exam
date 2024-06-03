#include "EnemyController.h"

#include "SceneManager.h"
#include "Scene.h"
#include "CollisionSystem.h"

#include "GameObject.h"
#include "TextureComponent.h"
#include "StatusComponent.h"
#include "ColliderComponent.h"

namespace enemyController
{
	void EnemyController::AddBalloomEnemy(int column, int row, glm::vec3 levelPosition)
	{
		auto levelScene = engine::sceneManager::sceneMap["Demo level"].get();

		auto go = std::make_unique<engine::GameObject>(glm::vec3(levelPosition.x + 16 * column, levelPosition.y + 16 * row, 0.f));
		go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "balloom.png"));
		go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
			go->GetComponent<engine::TextureComponent>()->GetTextureSize(), false, CollisionType::Enemy));
		
		auto sc = std::make_unique<StatusComponent>(go.get());
		sc->AddDataMapping("POINTS", 100);
		go->AddComponent<StatusComponent>(std::move(sc));

		levelScene->Add("balloom " + std::to_string(m_Count), std::move(go));
		
		++m_Count;
	}

	void EnemyController::KillEnemy(engine::GameObject* enemy)
	{
		--m_Count;

		NotifyObservers(engine::Event::EnemyDied, nullptr,
			enemy->GetComponent<StatusComponent>()->GetData("POINTS"));

		enemy->MarkDeletion();
	}
}