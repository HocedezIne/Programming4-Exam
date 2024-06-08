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
		return new LevelWonState();
	if (m_Lost)
		return new LevelLostState();

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
	engine::sceneManager::currentScenes[0]->GetObject("timer")->GetComponent<engine::TimerComponent>()->Reset();

	levelLoader::LoadLevel("../Data/STAGE " + std::to_string(m_CurrentLevel) + ".json");
	engine::sceneManager::currentScenes.push_back(engine::sceneManager::sceneMap["Level Dynamics"].get());

	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/LevelBackground.mp3", true);
	collisionSystem::collisionHandler.AddObserver(this);
	collisionSystem::collisionHandler.AddObserver(&BombController::GetInstance());

	// add player commands
	auto obj = engine::sceneManager::currentScenes[0]->GetObject("player1");
	if (obj != nullptr)
	{
		// reset player
		obj->SetLocalPosition(glm::vec3{ 16, 16, 0.f });

		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 0.f,-1.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ -1.f,0.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 0.f,1.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 1.f,0.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>(obj, 16));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_X, engine::KeyState::Pressed, std::make_unique<DetonateCommand>());
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_R, engine::KeyState::Pressed, std::make_unique<RespawnBombermanCommand>(obj));

		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadUp, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 0.f,-1.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadLeft, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ -1.f,0.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadDown, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 0.f,1.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadRight, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 1.f,0.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::A, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>(obj, 16));
		engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::B, engine::KeyState::Pressed, std::make_unique<DetonateCommand>());
	}
}

void LevelState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();

	engine::sceneManager::currentScenes[0]->EnableUpdates(false);
	engine::sceneManager::currentScenes[1]->EnableUpdates(false);

	collisionSystem::collisionHandler.RemoveObserver(this);
	collisionSystem::collisionHandler.RemoveObserver(&BombController::GetInstance());

	BombController::GetInstance().ClearBombCount();

	// remove player commands
	auto obj = engine::sceneManager::currentScenes[0]->GetObject("player1");
	if (obj != nullptr)
	{
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
	}
}

void LevelState::OnNotify(engine::Event event, void* caller, const std::any& /*args*/)
{
	if (event == engine::Event::PlayerDied)
	{
		m_Lost = true;
	}
	if (event == engine::Event::PlayerOnExit)
	{
		auto door = static_cast<engine::GameObject*>(caller);
		auto isDoorBlocked = std::any_cast<bool>(door->GetComponent<DataComponent>()->GetData("BLOCKED"));
		if (enemyController::EnemyController::GetInstance().GetCount() == 0 && !isDoorBlocked)
			m_Won = true;
	}
}