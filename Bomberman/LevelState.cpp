#include "GameStates.h"

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

	engine::sceneManager::currentScene = engine::sceneManager::sceneMap["Demo level"].get();
	engine::sceneManager::currentScene->EnableUpdates(true);
	engine::sceneManager::currentScene->GetObject("timer")->GetComponent<engine::TimerComponent>()->Reset();

	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/LevelBackground.mp3", true);
	collisionSystem::collisionHandler.AddObserver(this);
	collisionSystem::collisionHandler.AddObserver(&BombController::GetInstance());

	// add player commands
	auto obj = engine::sceneManager::currentScene->GetObject("player1");
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
	}
}

void LevelState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();

	engine::sceneManager::currentScene->EnableUpdates(false);

	collisionSystem::collisionHandler.RemoveObserver(this);
	collisionSystem::collisionHandler.RemoveObserver(&BombController::GetInstance());

	// remove player commands
	auto obj = engine::sceneManager::currentScene->GetObject("player1");
	if (obj != nullptr)
	{
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_X, engine::KeyState::Pressed);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_R, engine::KeyState::Pressed);
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