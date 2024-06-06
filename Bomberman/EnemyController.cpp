#include "EnemyController.h"

#include "SceneManager.h"
#include "Scene.h"
#include "CollisionSystem.h"
#include "ServiceLocator.h"

#include "GameObject.h"
#include "TextureComponent.h"
#include "DataComponent.h"
#include "ColliderComponent.h"

#include "StateComponent.h"
#include "BalloomBehaviour.h"
#include "DollBehaviour.h"

namespace enemyController
{
	template <typename StartState>
	void EnemyController::AddEnemy(int column, int row, const std::string& name, int points)
	{
		auto levelScene = engine::sceneManager::sceneMap["Demo level"].get();
		auto levelBg = levelScene->GetObject("bg");

		auto go = std::make_unique<engine::GameObject>(glm::vec3(16 * column, 16 * row, 0.f));
		go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), name + ".png"));
		go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
			go->GetComponent<engine::TextureComponent>()->GetTextureSize(), false, CollisionType::Enemy));
		go->AddComponent<StateComponent<EnemyStateInterface>>(std::make_unique<StateComponent<EnemyStateInterface>>(go.get(), new StartState(go.get()) ));

		auto sc = std::make_unique<DataComponent>(go.get());
		sc->AddDataMapping("POINTS", points);
		go->AddComponent<DataComponent>(std::move(sc));

		go->SetParent(levelBg, false);
		levelScene->Add(name + " " + std::to_string(m_Count), std::move(go));

		++m_Count;
	}

	void EnemyController::AddBalloomEnemy(int column, int row)
	{
		AddEnemy<BalloomWalking>(column, row, "balloom", 100);
	}

	void EnemyController::AddDollEnemy(int column, int row)
	{
		AddEnemy<DollWalking>(column, row, "doll", 400);
	}

	void EnemyController::KillEnemy(engine::GameObject* enemy)
	{
		--m_Count;

		NotifyObservers(engine::Event::EnemyDied, nullptr,
			enemy->GetComponent<DataComponent>()->GetData("POINTS"));

		enemy->MarkDeletion();

		if (m_Count == 0)
		{
			auto data = engine::sceneManager::sceneMap["Demo level"].get()->GetObject("door")->GetComponent<DataComponent>()->GetData("BLOCKED");
			auto result = std::any_cast<bool>(data);
			if(result == false)
				engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/BombermanDoorUnlock.wav", false);
		}
	}
}