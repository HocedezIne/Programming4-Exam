#include "BombController.h"

#include <memory>

#include "ServiceLocator.h"
#include "SceneManager.h"
#include "Scene.h"

#include "TimerComponent.h"
#include "TextureComponent.h"
#include "ColliderComponent.h"

void BombController::OnNotify(engine::Event event, void* caller, const std::any& /*args*/)
{
	switch (event)
	{
	case engine::Event::TimerFinished: {
		auto obj = static_cast<engine::GameObject*>(caller);
		
		if (obj->GetChildren().empty())
		{
			engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/BombermanExplosion.wav", false);

			obj->GetComponent<engine::TextureComponent>()->SetTexture("explosion center.png");
			obj->GetComponent<engine::TimerComponent>()->Reset();

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
					go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "explosion " + textureDirection + ".png"));
					go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
						go->GetComponent<engine::TextureComponent>()->GetTextureSize(), true, CollisionType::Explosion));
					go->SetParent(obj, false);
					engine::sceneManager::currentScene->Add("explosion " + std::to_string(idx) + "." + std::to_string(direction), std::move(go));
				}
			}
		}
		else
		{
			obj->MarkDeletion();
			--m_LiveBombs;
		}
		
		break;
	}
	default:
		break;
	}
}

void BombController::AddBomb(const glm::vec3 pos)
{
	if (m_LiveBombs < m_MaxBombCount)
	{
		++m_LiveBombs;

		// create bomb
		auto scene = engine::sceneManager::currentScene;
		auto bombwrapper = std::make_unique<engine::GameObject>();

		auto bomb = std::make_unique<engine::GameObject>(pos);
		bomb->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(bomb.get(), "bomb.png"));
		bomb->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(bomb.get(),
			bomb->GetComponent<engine::TextureComponent>()->GetTextureSize(), true, CollisionType::Block));
		auto timer = std::make_unique<engine::TimerComponent>(bomb.get(), 3);
		timer->AddObserver(this);
		bomb->AddComponent<engine::TimerComponent>(std::move(timer));
		scene->Add("bomb", std::move(bomb));
	}
}