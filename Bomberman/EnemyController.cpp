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
#include "ConstantWalkingBehaviour.h"
#include "ChasingBehaviour.h"

namespace enemyController
{
	template <typename StartState>
	void EnemyController::AddEnemy(int column, int row, const std::string& name, 
		int points, float speed, float chasingDistance)
	{
		auto levelScene = engine::sceneManager::sceneMap["Demo level"].get();
		auto levelBg = levelScene->GetObject("bg");

		auto go = std::make_unique<engine::GameObject>(glm::vec3(16 * column, 16 * row, 0.f));
		go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/" + name + ".png"));
		go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
			go->GetComponent<engine::TextureComponent>()->GetTextureSize(), false, CollisionType::Enemy));
		go->AddComponent<StateComponent<EnemyStateInterface>>(std::make_unique<StateComponent<EnemyStateInterface>>(go.get(),
			new StartState(speed, go.get(), m_Players, chasingDistance) ));

		auto sc = std::make_unique<DataComponent>(go.get());
		sc->AddDataMapping("POINTS", points);
		go->AddComponent<DataComponent>(std::move(sc));

		go->SetParent(levelBg, false);
		levelScene->Add(name + " " + std::to_string(m_Count), std::move(go));

		++m_Count;
	}

	void EnemyController::AddBalloomEnemy(int column, int row)
	{
		AddEnemy<ConstantWalking>(column, row, "balloom", 100, 25.f);
	}

	void EnemyController::AddOnealEnemy(int column, int row)
	{
		AddEnemy<WalkTillChasing>(column, row, "oneal", 200, 50.f, 5*16.f);
	}

	void EnemyController::AddDollEnemy(int column, int row)
	{
		AddEnemy<ConstantWalking>(column, row, "doll", 400, 50.f);
	}

	void EnemyController::AddMinvoEnemy(int column, int row)
	{
		AddEnemy<WalkTillChasing>(column, row, "minvo", 800, 75.f);
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
				engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/BombermanDoorUnlock.wav", false);
		}
	}
}