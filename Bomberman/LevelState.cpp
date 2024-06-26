#include "GameStates.h"

#include "LevelLoader.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

#include "CollisionSystem.h"
#include "EnemyController.h"

#include "BombCommands.h"
#include "RespawnBombermanCommand.h"

#include "TimerComponent.h"
#include "DataComponent.h"

GameStateInterface* LevelState::HandleInput()
{
	if (engine::InputCommandLinker::GetInstance().IsKeyDown(SDL_SCANCODE_F1) || m_Won)
	{
		return new LevelWonState();
	}
	if (m_Lost)
	{
		return new LevelLostState();
	}

	return nullptr;
}

void LevelState::Update()
{

}

void LevelState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(173, 173, 173));

	engine::sceneManager::currentScenes.push_back(engine::sceneManager::sceneMap["Level Statics"].get());
	engine::sceneManager::currentScenes[0]->EnableUpdates(true);
	auto timer = engine::sceneManager::currentScenes[0]->GetObject("timer")->GetComponent<engine::TimerComponent>();
	timer->Reset();
	timer->AddObserver(this);


	if (m_GameMode != GameMode::Vs)levelLoader::LoadLevel("../Data/STAGE " + std::to_string(m_CurrentLevel) + ".json");
	else levelLoader::LoadLevel("../Data/Versus.json");
	engine::sceneManager::currentScenes.push_back(engine::sceneManager::sceneMap["Level Dynamics"].get());

	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/LevelBackground.mp3", true);
	collisionSystem::collisionHandler.AddObserver(this);

	// add player commands
	switch (m_GameMode)
	{
	case GameMode::Vs:
	{
		auto p1 = engine::sceneManager::currentScenes[0]->GetObject("player1");

		// reset player
		p1->SetLocalPosition(glm::vec3{ levelLoader::m_GridSize, levelLoader::m_GridSize, 0.f });

		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 0.f,-1.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ -1.f,0.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 0.f,1.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 1.f,0.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>(p1));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_X, engine::KeyState::Pressed, std::make_unique<DetonateCommand>(p1));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_R, engine::KeyState::Pressed, std::make_unique<RespawnBombermanCommand>(p1));

		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadUp, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 0.f,-1.f,0.f }, 100.f), 1);
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadLeft, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ -1.f,0.f,0.f }, 100.f), 1);
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadDown, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 0.f,1.f,0.f }, 100.f), 1);
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadRight, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 1.f,0.f,0.f }, 100.f), 1);
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::A, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>(p1), 1);
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::B, engine::KeyState::Pressed, std::make_unique<DetonateCommand>(p1), 1);

		auto p2 = engine::sceneManager::currentScenes[0]->GetObject("player2");

		// reset player
		p2->SetLocalPosition(glm::vec3{ levelLoader::m_GridSize*18, levelLoader::m_GridSize*10, 0.f });

		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadUp, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p2, glm::vec3{ 0.f,-1.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadLeft, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p2, glm::vec3{ -1.f,0.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadDown, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p2, glm::vec3{ 0.f,1.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadRight, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p2, glm::vec3{ 1.f,0.f,0.f }, 100.f));
		break;
	}

	case GameMode::Coop:
	{
		auto p1 = engine::sceneManager::currentScenes[0]->GetObject("player1");

		// reset player
		p1->SetLocalPosition(glm::vec3{ levelLoader::m_GridSize, levelLoader::m_GridSize, 0.f });

		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 0.f,-1.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ -1.f,0.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 0.f,1.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 1.f,0.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>(p1));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_X, engine::KeyState::Pressed, std::make_unique<DetonateCommand>(p1));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_R, engine::KeyState::Pressed, std::make_unique<RespawnBombermanCommand>(p1));

		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadUp, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 0.f,-1.f,0.f }, 100.f), 1);
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadLeft, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ -1.f,0.f,0.f }, 100.f), 1);
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadDown, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 0.f,1.f,0.f }, 100.f), 1);
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadRight, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 1.f,0.f,0.f }, 100.f), 1);
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::A, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>(p1), 1);
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::B, engine::KeyState::Pressed, std::make_unique<DetonateCommand>(p1), 1);

		auto p2 = engine::sceneManager::currentScenes[0]->GetObject("player2");

		// reset player
		p2->SetLocalPosition(glm::vec3{ levelLoader::m_GridSize*2, levelLoader::m_GridSize, 0.f });

		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadUp, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p2, glm::vec3{ 0.f,-1.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadLeft, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p2, glm::vec3{ -1.f,0.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadDown, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p2, glm::vec3{ 0.f,1.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadRight, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p2, glm::vec3{ 1.f,0.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::A, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>(p2));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::B, engine::KeyState::Pressed, std::make_unique<DetonateCommand>(p2));
		break;
	}

	case GameMode::Single:
	{
		auto p1 = engine::sceneManager::currentScenes[0]->GetObject("player1");

		// reset player
		p1->SetLocalPosition(glm::vec3{ levelLoader::m_GridSize, levelLoader::m_GridSize, 0.f });

		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 0.f,-1.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ -1.f,0.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 0.f,1.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 1.f,0.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>(p1));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_X, engine::KeyState::Pressed, std::make_unique<DetonateCommand>(p1));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_R, engine::KeyState::Pressed, std::make_unique<RespawnBombermanCommand>(p1));

		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadUp, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 0.f,-1.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadLeft, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ -1.f,0.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadDown, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 0.f,1.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadRight, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(p1, glm::vec3{ 1.f,0.f,0.f }, 100.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::A, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>(p1));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::B, engine::KeyState::Pressed, std::make_unique<DetonateCommand>(p1));
		break;
	}
	default:
		break;
	}
}

void LevelState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();

	engine::sceneManager::currentScenes[0]->EnableUpdates(false);
	engine::sceneManager::currentScenes[1]->EnableUpdates(false);

	collisionSystem::collisionHandler.RemoveObserver(this);
	enemyController::EnemyController::GetInstance().ClearEnemies();

	engine::sceneManager::currentScenes[0]->GetObject("timer")->GetComponent<engine::TimerComponent>()->RemoveObserver(this);

	switch (m_GameMode)
	{
	case GameMode::Coop:
		engine::sceneManager::currentScenes[0]->GetObject("player2")->GetComponent<BombControllerComponent>()->ClearBombCount();

	case GameMode::Vs:
	case GameMode::Single:
		engine::sceneManager::currentScenes[0]->GetObject("player1")->GetComponent<BombControllerComponent>()->ClearBombCount();
		break;
	
	default:
		break;
	}

	// remove player commands
	switch (m_GameMode)
	{
	case GameMode::Coop:
	case GameMode::Vs:
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_X, engine::KeyState::Pressed);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_R, engine::KeyState::Pressed);

		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadLeft, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadRight, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadUp, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadDown, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::A, engine::KeyState::Pressed);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::B, engine::KeyState::Pressed);

		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadLeft, engine::KeyState::Held, 1);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadRight, engine::KeyState::Held, 1);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadUp, engine::KeyState::Held, 1);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadDown, engine::KeyState::Held, 1);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::A, engine::KeyState::Pressed, 1);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::B, engine::KeyState::Pressed, 1);

		break;
	case GameMode::Single:
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_X, engine::KeyState::Pressed);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_R, engine::KeyState::Pressed);

		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadLeft, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadRight, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadUp, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadDown, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::A, engine::KeyState::Pressed);
		engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::B, engine::KeyState::Pressed);
	default:
		break;
	}
}

void LevelState::OnNotify(engine::Event event, void* caller, const std::any& /*args*/)
{
	if (event == engine::Event::PlayerDied)
	{
		m_Lost = true;
	}
	if (event == engine::Event::PlayableEnemyDied)
	{
		m_Won = true;
	}
	if (event == engine::Event::PlayerOnExit)
	{
		auto door = static_cast<engine::GameObject*>(caller);
		auto isDoorBlocked = std::any_cast<bool>(door->GetComponent<DataComponent>()->GetData("BLOCKED"));
		if (enemyController::EnemyController::GetInstance().GetCount() == 0 && !isDoorBlocked && m_GameMode != GameMode::Vs)
			m_Won = true;
	}
	if (event == engine::Event::TimerFinished)
	{
		m_Lost = true;
		auto p1dc = engine::sceneManager::currentScenes[0]->GetObject("player1")->GetComponent<DataComponent>();
		p1dc->UpdateData("LEFT", std::any_cast<int>(p1dc->GetData("LEFT")) - 1);
		if (m_GameMode != GameMode::Single)
		{
			auto p2dc = engine::sceneManager::currentScenes[0]->GetObject("player2")->GetComponent<DataComponent>();
			p2dc->UpdateData("LEFT", std::any_cast<int>(p2dc->GetData("LEFT")) - 1);
		}
	}
}