#include "BombControllerComponent.h"

#include <memory>

#include "ServiceLocator.h"
#include "SceneManager.h"
#include "Scene.h"

#include "ColliderComponent.h"
#include "DataComponent.h"
#include "TimerComponent.h"
#include "TextureComponent.h"

void BombControllerComponent::OnNotify(engine::Event event, void* caller, const std::any& /*args*/)
{
	switch (event)
	{
	case engine::Event::TimerFinished: {
		auto obj = static_cast<engine::GameObject*>(caller);
		
		if (obj->GetChildren().empty())
		{
			ExplodeBomb(obj);
		}
		else
		{
			obj->MarkDeletion();
			m_Bombs.erase(std::remove(m_Bombs.begin(), m_Bombs.end(), obj), m_Bombs.end());
			--m_LiveBombs;
		}
		break;
	}

	case engine::Event::PowerUpCollected:
	{
		auto obj = static_cast<engine::GameObject*>(caller);
		auto type = std::any_cast<std::string>(obj->GetComponent<DataComponent>()->GetData("TYPE"));

		if (type == "FLAME UP")
			++m_ExplosionSize;
		else if (type == "BOMB UP")
			++m_MaxBombCount;
		else if (type == "REMOTE CONTROL")
			m_RemoteEnabled = true;

		break;
	}

	default:
		break;
	}
}

void BombControllerComponent::ResetAllPowerups()
{
	m_ExplosionSize = m_DefaultExplosionSize;
	m_MaxBombCount = m_DefaultMaxBombCount;
}

void BombControllerComponent::AddBomb(const glm::vec3 pos)
{
	if (m_LiveBombs < m_MaxBombCount)
	{
		++m_LiveBombs;

		engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/BombermanDropsBomb.wav", false);

		// create bomb
		auto scene = engine::sceneManager::currentScenes[1];
		auto bombwrapper = std::make_unique<engine::GameObject>();

		auto bomb = std::make_unique<engine::GameObject>(pos);
		bomb->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(bomb.get(), "Images/bomb.png"));
		bomb->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(bomb.get(),
			bomb->GetComponent<engine::TextureComponent>()->GetTextureSize(), true, CollisionType::Block));

		if (!m_RemoteEnabled)
		{
			auto timer = std::make_unique<engine::TimerComponent>(bomb.get(), 3);
			timer->AddObserver(this);
			bomb->AddComponent<engine::TimerComponent>(std::move(timer));
		}

		m_Bombs.push_back(bomb.get());

		scene->Add("bomb " + std::to_string(rand()%256), std::move(bomb));
	}
}

void BombControllerComponent::ExplodeBomb(engine::GameObject* obj)
{
	if (obj->GetChildren().empty())
	{
		engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/BombermanExplosion.wav", false);

		if (m_RemoteEnabled)
		{
			auto timer = std::make_unique<engine::TimerComponent>(obj, 3);
			timer->AddObserver(this);
			obj->AddComponent<engine::TimerComponent>(std::move(timer));
		}
		else obj->GetComponent<engine::TimerComponent>()->Reset();
		obj->GetComponent<engine::TextureComponent>()->SetTexture("Images/explosion center.png");

		auto bombIdentifier = engine::sceneManager::currentScenes[1]->GetObjectIdentifier(obj);
		// create explosion
		for (int idx{ 1 }; idx < m_ExplosionSize + 1; ++idx)
		{
			for (int direction{}; direction < 4; ++direction)
			{
				glm::vec3 pos;
				switch (direction)
				{
				case 0: pos = glm::vec3{ m_GridSize * idx, 0, 0 }; break;
				case 1: pos = glm::vec3{ 0, m_GridSize * idx, 0 }; break;
				case 2: pos = glm::vec3{ -m_GridSize * idx, 0, 0 }; break;
				case 3: pos = glm::vec3{ 0, -m_GridSize * idx, 0 }; break;
				}

				std::string textureDirection = (direction % 2 == 0) ? "horizontal" : "vertical";
				auto go = std::make_unique<engine::GameObject>(pos);
				go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "Images/explosion " + textureDirection + ".png"));
				go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
					go->GetComponent<engine::TextureComponent>()->GetTextureSize(), false, CollisionType::Explosion));
				auto dc = std::make_unique<DataComponent>(go.get());
				dc->AddDataMapping("PLAYER", GetOwner());
				go->AddComponent(std::move(dc));

				go->SetParent(obj, false);
				engine::sceneManager::currentScenes[1]->Add(bombIdentifier + " explosion " + std::to_string(idx) + "." + std::to_string(direction), std::move(go));
			}
		}
	}
}