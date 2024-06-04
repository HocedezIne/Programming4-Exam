#include "GameStates.h"
#include "InputCommandLinker.h"
#include "BombCommand.h"
#include "RespawnBombermanCommand.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "CollisionSystem.h"
#include "Renderer.h"
#include "TimeService.h"
#include "StatusComponent.h"
#include "EnemyController.h"

#pragma region StartMenuState
GameStateInterface* StartMenuState::HandleInput()
{
	auto& input = engine::InputCommandLinker::GetInstance();

	if (input.IsKeyDown(SDL_SCANCODE_RETURN))
	{
		if(m_Cursor->GetLocalPosition() == m_Start) return new LevelLoadingState();
		else if(m_Cursor->GetLocalPosition() == m_Controls) return new ControlsMenuState();
	}

	return nullptr;
}

void StartMenuState::Update()
{
		
}

void StartMenuState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScene =  engine::sceneManager::sceneMap["Start menu"].get();
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/StartMenu.mp3", true);

	m_Cursor = engine::sceneManager::currentScene->GetObject("cursor");
	assert(m_Cursor);
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Pressed, std::make_unique<engine::SnapToInputCommand>(m_Cursor, m_Start));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Pressed, std::make_unique<engine::SnapToInputCommand>(m_Cursor, m_Controls));
}

void StartMenuState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}
#pragma endregion StartMenuState

#pragma region LevelState
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
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/LevelBackground.mp3", true);
	collisionSystem::collisionHandler.AddObserver(this);

	// add player commands
	auto obj = engine::sceneManager::currentScene->GetObject("player1");
	if (obj != nullptr)
	{
		// reset player
		obj->SetLocalPosition(glm::vec3{ 16, 16, 0.f });
		obj->DoesUpdate(true);

		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 0.f,-1.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ -1.f,0.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 0.f,1.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 1.f,0.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>(obj, 16));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_R, engine::KeyState::Pressed, std::make_unique<RespawnBombermanCommand>(obj));
	}
}

void LevelState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();

	// remove player commands
	auto obj = engine::sceneManager::currentScene->GetObject("player1");
	if (obj != nullptr)
	{
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_R, engine::KeyState::Pressed);
	}
}

void LevelState::OnNotify(engine::Event event, void* /*caller*/, const std::any& /*args*/)
{
	if (event == engine::Event::PlayerDied)
	{
		m_Lost = true;
	}
	if (event == engine::Event::PlayerOnExit)
	{
		if (enemyController::EnemyController::GetInstance().GetCount() == 0)
			m_Won = true;
	}
}
#pragma endregion LevelState

#pragma region LevelLoadingState
GameStateInterface* LevelLoadingState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
		return new LevelState();

	return nullptr;
}

void LevelLoadingState::Update()
{
	m_TimeToStateSwitch -= engine::TimeService::GetInstance().GetDeltaTime();
}

void LevelLoadingState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScene = engine::sceneManager::sceneMap["Level loading"].get();
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/LevelStart.mp3", true);
}

void LevelLoadingState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}
#pragma endregion LevelLoadingState

#pragma region LevelLostState
GameStateInterface* LevelLostState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
		if (std::any_cast<int>(engine::sceneManager::currentScene->GetObject("player1")->GetComponent<StatusComponent>()->GetData("LEFT")) > 0)
			return new LevelLoadingState();
		else
			return new GameOverState();

	return nullptr;
}

void LevelLostState::Update()
{
	m_TimeToStateSwitch -= engine::TimeService::GetInstance().GetDeltaTime();
	if((m_TimeToStateSwitch-3.f) < 1e-5f) engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/LevelLost.mp3", true);
}

void LevelLostState::OnEnter()
{
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/BombermanDeath.wav", false);
}

void LevelLostState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}
#pragma endregion LevelLostState

#pragma region LevelWonState
GameStateInterface* LevelWonState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
		return new LevelLoadingState();

	return nullptr;
}

void LevelWonState::Update()
{
	m_TimeToStateSwitch -= engine::TimeService::GetInstance().GetDeltaTime();
}

void LevelWonState::OnEnter()
{
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/LevelFinished.mp3", true);
}

void LevelWonState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}
#pragma endregion LevelWonState

#pragma region GameOverState
GameStateInterface* GameOverState::HandleInput()
{
	return nullptr;
}

void GameOverState::Update()
{

}

void GameOverState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScene = engine::sceneManager::sceneMap["Game over"].get();
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/GameOver.mp3", true);
}

void GameOverState::OnExit()
{

}
#pragma endregion GameOverState

#pragma region ControlsMenuState
GameStateInterface* ControlsMenuState::HandleInput()
{
	if (engine::InputCommandLinker::GetInstance().IsKeyDown(SDL_SCANCODE_RETURN))
	{
		return new StartMenuState();
	}

	return nullptr;
}

void ControlsMenuState::Update()
{

}

void ControlsMenuState::OnEnter()
{
	engine::sceneManager::currentScene = engine::sceneManager::sceneMap["Controls menu"].get();
}

void ControlsMenuState::OnExit()
{
		
}
#pragma endregion ControlsMenuState