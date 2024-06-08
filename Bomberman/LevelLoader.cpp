#include "LevelLoader.h"

#include <fstream>
#include <iostream>

#include "SceneManager.h"

#include "EnemyController.h"

#include "ColliderComponent.h"
#include "DataComponent.h"
#include "TextureComponent.h"

namespace levelLoader
{
	std::vector<glm::vec2> m_UsedPositions{};

	void LoadLevel(const std::string filePath)
	{
		m_UsedPositions.clear();

		m_UsedPositions.push_back(glm::vec2{ 16.f, 16.f }); // bombermans position
		// indestructible blocks
		for (int r{}; r < 5; ++r)
		{
			glm::vec2 pos{ };
			pos.y = (r + 1) * 32.f;
			for (int c{}; c < 14; ++c)
			{
				pos.x = (c + 1) * 32.f;
				m_UsedPositions.push_back(pos);
			}
		}

		auto parent = engine::sceneManager::sceneMap["Level Statics"].get()->GetObject("bg");
		auto& levelDynamicScene = engine::sceneManager::CreateScene("Level Dynamics");

		// generated per level
		srand(static_cast<unsigned int>(time(NULL)));
		std::ifstream f(filePath);

		if (f.is_open())
		{
			json j;
			f >> j;

			CreateDoor(parent, levelDynamicScene);
			CreatePowerUp(parent, levelDynamicScene, j["power up"]);
			CreateSoftBlocks(parent, levelDynamicScene, j["soft blocks count"]);
			CreateEnemies(parent, levelDynamicScene, j["enemies"]);

			f.close();
		}
		else
		{
			std::cout << "Issue with " << filePath << " couldn't load level\n";
		}
	}

	static glm::vec2 GetFreePosition()
	{
		glm::vec2 pos{};
		bool foundFree{ false };
		do
		{
			int randX = rand() % 29 +1;
			int randY = rand() % 11 +1;
			pos = { randX * 16.f, randY * 16.f };

			foundFree = std::find(m_UsedPositions.begin(), m_UsedPositions.end(), pos) == m_UsedPositions.end();
			if(pos == glm::vec2{ 32.f,16.f } || pos == glm::vec2{ 16.f, 32.f }) foundFree = false;
		} while (!foundFree);

		m_UsedPositions.push_back(pos);
		return pos;
	}

	static void CreateSoftBlocks(engine::GameObject* parent, engine::Scene& scene, int count)
	{
		for (int posIdx{static_cast<int>(m_UsedPositions.size()-3)}; posIdx < static_cast<int>(m_UsedPositions.size()); ++posIdx)
		{
			auto go = std::make_unique<engine::GameObject>(glm::vec3{ m_UsedPositions[posIdx], 0.f });
			go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/destructible.png"));
			go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
				go->GetComponent<engine::TextureComponent>()->GetTextureSize(), true, CollisionType::Destructable));
			go->SetParent(parent, false);
			scene.Add("destructible covering" + std::to_string(posIdx), std::move(go));
		}

		for (int idx = 0; idx < count-2; idx++)
		{
			glm::vec3 pos{ GetFreePosition(), 0 };

			auto go = std::make_unique<engine::GameObject>(pos);
			go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/destructible.png"));
			go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
				go->GetComponent<engine::TextureComponent>()->GetTextureSize(), true, CollisionType::Destructable));
			go->SetParent(parent, false);
			scene.Add("destructible " + std::to_string(idx), std::move(go));
		}
	}

	static void CreateDoor(engine::GameObject* parent, engine::Scene& scene)
	{
		glm::vec3 pos{ GetFreePosition(), 0 };

		auto go = std::make_unique<engine::GameObject>(pos);
		go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/door.png"));
		go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
			go->GetComponent<engine::TextureComponent>()->GetTextureSize(), true, CollisionType::Door));
		auto sc = std::make_unique<DataComponent>(go.get());
		sc->AddDataMapping("BLOCKED", true);
		go->AddComponent<DataComponent>(std::move(sc));
		go->SetParent(parent, false);
		scene.Add("door", std::move(go));
	}

	static void CreatePowerUp(engine::GameObject* parent, engine::Scene& scene, std::string type)
	{
		glm::vec3 pos{ GetFreePosition(), 0 };

		std::string lowercaseType{};
		lowercaseType.resize(type.size());
		std::transform(type.begin(), type.end(), lowercaseType.begin(),
			[](unsigned char c) { return static_cast<char>(std::tolower(c)); });

		auto go = std::make_unique<engine::GameObject>(pos);
		go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/" + lowercaseType + ".png"));
		go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
			go->GetComponent<engine::TextureComponent>()->GetTextureSize(), true, CollisionType::PowerUp));
		auto sc = std::make_unique<DataComponent>(go.get());
		sc->AddDataMapping("BLOCKED", true);
		sc->AddDataMapping("TYPE", type);
		go->AddComponent<DataComponent>(std::move(sc));
		go->SetParent(parent, false);
		scene.Add(lowercaseType, std::move(go));
	}

	static void CreateEnemies(engine::GameObject* parent, engine::Scene& scene, const json& enemiesCollection)
	{
		auto& ec = enemyController::EnemyController::GetInstance();
		for (const json& enemyType : enemiesCollection)
		{
			for (int idx{}; idx < enemyType["count"]; ++idx)
			{
				glm::vec3 pos{ GetFreePosition(), 0.f };

				if (enemyType["type"] == "balloom")
					ec.AddBalloomEnemy(pos, parent, scene);
				else if (enemyType["type"] == "oneal")
					ec.AddOnealEnemy(pos, parent, scene);
				else if (enemyType["type"] == "doll")
					ec.AddDollEnemy(pos, parent, scene);
				else if (enemyType["type"] == "minvo")
					ec.AddMinvoEnemy(pos, parent, scene);
			}
		}
	}
}