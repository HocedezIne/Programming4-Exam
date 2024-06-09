#include "LevelLoader.h"

#include <fstream>
#include <iostream>

#include "SceneManager.h"
#include "ResourceManager.h"

#include "CollisionSystem.h"
#include "EnemyController.h"

#include "BombControllerComponent.h"
#include "ColliderComponent.h"
#include "DataComponent.h"
#include "FPSComponent.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "TimerComponent.h"
#include "UILinkingComponent.h"

namespace levelLoader
{
	const float m_GridSize = 32.f;
	const glm::vec2 m_Scale = { 2.f,2.f };
	std::vector<glm::vec2> m_UsedPositions{};

	static void LoadRequiredPlayers(GameMode gameMode)
	{
		std::vector<engine::GameObject*> players;
		auto scene = engine::sceneManager::sceneMap["Level Statics"].get();
		auto parent = scene->GetObject("bg");
		auto font = engine::ResourceManager::GetInstance().LoadFont("Fonts/nes-arcade-font-monospace.otf", 16);

		switch (gameMode)
		{
		case GameMode::Coop:
		{
			auto go = std::make_unique<engine::GameObject>(glm::vec3{ m_GridSize*2, m_GridSize, 0.f });
			go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/bomberman 2.png", glm::vec2{2.f,2.f}));
			go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
				go->GetComponent<engine::TextureComponent>()->GetTextureSize(), false, CollisionType::Player));

			auto bc = std::make_unique<BombControllerComponent>(go.get());
			collisionSystem::collisionHandler.AddObserver(bc.get());
			go->AddComponent(std::move(bc));

			auto sc = std::make_unique<DataComponent>(go.get());
			sc->AddDataMapping("LEFT", 3);
			sc->AddDataMapping("SCORE", 0);
			collisionSystem::collisionHandler.AddObserver(sc.get());
			enemyController::EnemyController::GetInstance().AddObserver(sc.get());

			auto goUI = std::make_unique<engine::GameObject>(glm::vec3(500.f, 40.f, 0.f));
			goUI->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(goUI.get(), "", font));
			goUI->AddComponent<UILinkingComponent>(std::make_unique<UILinkingComponent>(goUI.get(), "LEFT", sc.get()));
			collisionSystem::collisionHandler.AddObserver(goUI->GetComponent<UILinkingComponent>());
			scene->Add("player2 lives", std::move(goUI));

			goUI = std::make_unique<engine::GameObject>(glm::vec3(350.f, 40.f, 0.f));
			goUI->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(goUI.get(), "", font));
			goUI->AddComponent<UILinkingComponent>(std::make_unique<UILinkingComponent>(goUI.get(), "SCORE", sc.get(), StringFormat{ 2, '0', false }));
			enemyController::EnemyController::GetInstance().AddObserver(goUI->GetComponent<UILinkingComponent>());
			scene->Add("player2 score", std::move(goUI));

			go->AddComponent<DataComponent>(std::move(sc));
			go->SetParent(parent, false);
			players.push_back(go.get());
			scene->Add("player2", std::move(go));
		}

		case GameMode::Single:
		{
			auto go = std::make_unique<engine::GameObject>(glm::vec3{ m_GridSize, m_GridSize, 0.f });
			go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/bomberman.png", m_Scale));
			go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
				go->GetComponent<engine::TextureComponent>()->GetTextureSize(), false, CollisionType::Player));

			auto bc = std::make_unique<BombControllerComponent>(go.get());
			collisionSystem::collisionHandler.AddObserver(bc.get());
			go->AddComponent(std::move(bc));

			auto sc = std::make_unique<DataComponent>(go.get());
			sc->AddDataMapping("LEFT", 3);
			sc->AddDataMapping("SCORE", 0);
			collisionSystem::collisionHandler.AddObserver(sc.get());
			enemyController::EnemyController::GetInstance().AddObserver(sc.get());

			auto goUI = std::make_unique<engine::GameObject>(glm::vec3(500.f, 12.f, 0.f));
			goUI->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(goUI.get(), "", font));
			goUI->AddComponent<UILinkingComponent>(std::make_unique<UILinkingComponent>(goUI.get(), "LEFT", sc.get()));
			collisionSystem::collisionHandler.AddObserver(goUI->GetComponent<UILinkingComponent>());
			scene->Add("player1 lives", std::move(goUI));

			goUI = std::make_unique<engine::GameObject>(glm::vec3(350.f, 12.f, 0.f));
			goUI->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(goUI.get(), "", font));
			goUI->AddComponent<UILinkingComponent>(std::make_unique<UILinkingComponent>(goUI.get(), "SCORE", sc.get(), StringFormat{ 2, '0', false }));
			enemyController::EnemyController::GetInstance().AddObserver(goUI->GetComponent<UILinkingComponent>());
			scene->Add("player1 score", std::move(goUI));

			go->AddComponent<DataComponent>(std::move(sc));
			go->SetParent(parent, false);
			players.push_back(go.get());
			scene->Add("player1", std::move(go));

			break;
		}
		case GameMode::Vs:
		{
			auto go = std::make_unique<engine::GameObject>(glm::vec3{ m_GridSize, m_GridSize, 0.f });
			go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/bomberman.png", m_Scale));
			go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
				go->GetComponent<engine::TextureComponent>()->GetTextureSize(), false, CollisionType::Player));

			auto bc = std::make_unique<BombControllerComponent>(go.get());
			collisionSystem::collisionHandler.AddObserver(bc.get());
			go->AddComponent(std::move(bc));

			auto sc = std::make_unique<DataComponent>(go.get());
			sc->AddDataMapping("LEFT", 3);
			collisionSystem::collisionHandler.AddObserver(sc.get());
			enemyController::EnemyController::GetInstance().AddObserver(sc.get());

			auto goUI = std::make_unique<engine::GameObject>(glm::vec3(500.f, 12.f, 0.f));
			goUI->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(goUI.get(), "", font));
			goUI->AddComponent<UILinkingComponent>(std::make_unique<UILinkingComponent>(goUI.get(), "LEFT", sc.get()));
			collisionSystem::collisionHandler.AddObserver(goUI->GetComponent<UILinkingComponent>());
			scene->Add("player1 lives", std::move(goUI));

			go->AddComponent<DataComponent>(std::move(sc));
			go->SetParent(parent, false);
			players.push_back(go.get());
			scene->Add("player1", std::move(go));

			go = std::make_unique<engine::GameObject>(glm::vec3{ m_GridSize, 40.f, 0.f });
			go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/balloom.png", m_Scale));
			go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
				go->GetComponent<engine::TextureComponent>()->GetTextureSize(), false, CollisionType::PlayableEnemy));

			sc = std::make_unique<DataComponent>(go.get());
			sc->AddDataMapping("LEFT", 3);
			collisionSystem::collisionHandler.AddObserver(sc.get());
			enemyController::EnemyController::GetInstance().AddObserver(sc.get());

			goUI = std::make_unique<engine::GameObject>(glm::vec3(500.f, 40.f, 0.f));
			goUI->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(goUI.get(), "", font));
			goUI->AddComponent<UILinkingComponent>(std::make_unique<UILinkingComponent>(goUI.get(), "LEFT", sc.get()));
			collisionSystem::collisionHandler.AddObserver(goUI->GetComponent<UILinkingComponent>());
			scene->Add("player2 lives", std::move(goUI));

			go->AddComponent<DataComponent>(std::move(sc));
			go->SetParent(parent, false);
			players.push_back(go.get());
			scene->Add("player2", std::move(go));
			break;
		}
		default:
			break;
		}

		enemyController::EnemyController::GetInstance().SetPlayers(players);
	}

	void LoadLevel(const std::string filePath)
	{
		m_UsedPositions.clear();

		m_UsedPositions.push_back(glm::vec2{ m_GridSize, m_GridSize }); // bombermans position
		// indestructible blocks
		for (int r{}; r < 5; ++r)
		{
			glm::vec2 pos{ };
			pos.y = (r + 1) * m_GridSize*2;
			for (int c{}; c < 14; ++c)
			{
				pos.x = (c + 1) * m_GridSize*2;
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

	void LoadStatics(GameMode gameMode)
	{
		auto& LevelStaticScene = engine::sceneManager::CreateScene("Level Statics");

		auto font = engine::ResourceManager::GetInstance().LoadFont("Fonts/nes-arcade-font-monospace.otf", 16);

#if NDEBUG
#else
		auto fps = std::make_unique<engine::GameObject>(glm::vec3{ 175.f, 12.f, 0.f });
		fps->AddComponent<engine::FPSComponent>(std::make_unique<engine::FPSComponent>(fps.get()));
		LevelStaticScene.Add("fps", std::move(fps));
#endif

		auto go = std::make_unique<engine::GameObject>(glm::vec3{ 10.f, 12.f, 0.f });
		go->AddComponent<engine::TimerComponent>(std::make_unique<engine::TimerComponent>(go.get(), 200, true));
		LevelStaticScene.Add("timer", std::move(go));

		auto lvlbg = std::make_unique<engine::GameObject>(glm::vec3{ -m_GridSize/2, 64.f,0.f });
		lvlbg->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(lvlbg.get(), "Images/playfield short.png", m_Scale));

		// outer bounds
		{
			auto lvlbound = std::make_unique<engine::GameObject>();
			lvlbound->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(lvlbound.get(),
				glm::vec2{ lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().x, m_GridSize }, true, CollisionType::Wall));
			lvlbound->SetParent(lvlbg.get(), false);
			LevelStaticScene.Add("top bound", std::move(lvlbound));

			lvlbound = std::make_unique<engine::GameObject>();
			lvlbound->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(lvlbound.get(),
				glm::vec2{ m_GridSize, lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().y }, true, CollisionType::Wall));
			lvlbound->SetParent(lvlbg.get(), false);
			LevelStaticScene.Add("left bound", std::move(lvlbound));

			lvlbound = std::make_unique<engine::GameObject>(glm::vec3{ 0.f, lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().y - m_GridSize ,0.f });
			lvlbound->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(lvlbound.get(),
				glm::vec2{ lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().x, m_GridSize }, true, CollisionType::Wall));
			lvlbound->SetParent(lvlbg.get(), false);
			LevelStaticScene.Add("bottom bound", std::move(lvlbound));

			lvlbound = std::make_unique<engine::GameObject>(glm::vec3{ lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().x - m_GridSize,0.f,0.f });
			lvlbound->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(lvlbound.get(),
				glm::vec2{ m_GridSize, lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().y }, true, CollisionType::Wall));
			lvlbound->SetParent(lvlbg.get(), false);
			LevelStaticScene.Add("right bound", std::move(lvlbound));
		}

		auto bgptr = lvlbg.get();
		LevelStaticScene.Add("bg", std::move(lvlbg));

		// inner blocks
		for (int r{}; r < 5; ++r)
		{
			glm::vec3 pos{ };
			pos.y = (r + 1) * m_GridSize*2;
			for (int c{}; c < 9; ++c)
			{
				pos.x = (c + 1) * m_GridSize*2;
				auto lvlbound = std::make_unique<engine::GameObject>(pos);
				lvlbound->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(lvlbound.get(),
					glm::vec2{ m_GridSize, m_GridSize }, true, CollisionType::Block));
				lvlbound->SetParent(bgptr, false);
				LevelStaticScene.Add("r" + std::to_string(r + 1) + "c" + std::to_string(c + 1), std::move(lvlbound));
			}
		}

		LoadRequiredPlayers(gameMode);
	}

	static glm::vec2 GetFreePosition()
	{
		glm::vec2 pos{};
		bool foundFree{ false };
		do
		{
			int randX = rand() % 19 +1;
			int randY = rand() % 11 +1;
			pos = { randX * m_GridSize, randY * m_GridSize };

			foundFree = std::find(m_UsedPositions.begin(), m_UsedPositions.end(), pos) == m_UsedPositions.end();
			if(pos == glm::vec2{ m_GridSize*2,m_GridSize } || pos == glm::vec2{ m_GridSize, m_GridSize*2 }) foundFree = false;
		} while (!foundFree);

		m_UsedPositions.push_back(pos);
		return pos;
	}

	static void CreateSoftBlocks(engine::GameObject* parent, engine::Scene& scene, int count)
	{
		for (int posIdx{static_cast<int>(m_UsedPositions.size()-2)}; posIdx < static_cast<int>(m_UsedPositions.size()); ++posIdx)
		{
			auto go = std::make_unique<engine::GameObject>(glm::vec3{ m_UsedPositions[posIdx], 0.f });
			go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/destructible.png", m_Scale));
			go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
				go->GetComponent<engine::TextureComponent>()->GetTextureSize(), true, CollisionType::Destructable));
			go->SetParent(parent, false);
			scene.Add("destructible covering" + std::to_string(posIdx), std::move(go));
		}

		for (int idx = 0; idx < count-2; idx++)
		{
			glm::vec3 pos{ GetFreePosition(), 0 };

			auto go = std::make_unique<engine::GameObject>(pos);
			go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/destructible.png", m_Scale));
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
		go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/door.png", m_Scale));
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
		go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/" + lowercaseType + ".png", m_Scale));
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