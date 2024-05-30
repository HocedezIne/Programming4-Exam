#include "GameStates.h"
#include "InputCommandLinker.h"
#include "BombermanCommands.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "Renderer.h"
#include "TimeService.h"

#pragma region StartMenuState
GameStateInterface* StartMenuState::HandleInput()
{
	auto& input = engine::InputCommandLinker::GetInstance();

	if (input.IsKeyDown(SDL_SCANCODE_Z) )
		return new LevelLoadingState();
	if (input.IsKeyDown(SDL_SCANCODE_X))
		return new ControlsMenuState();

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
}

void StartMenuState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}
#pragma endregion StartMenuState

#pragma region LevelState
GameStateInterface* LevelState::HandleInput()
{
	if (engine::InputCommandLinker::GetInstance().IsKeyDown(SDL_SCANCODE_F1))
	{
		return new LevelLoadingState();
	}

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

	// add player commands
	auto obj = engine::sceneManager::currentScene->GetObject("player1");
	if (obj != nullptr)
	{
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 0.f,-1.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ -1.f,0.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 0.f,1.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 1.f,0.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>(obj));
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

	}
}
#pragma endregion LevelState

#pragma region LevelLoading
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
#pragma endregion LevelLoading

#pragma region ControlsMenu
GameStateInterface* ControlsMenuState::HandleInput()
{
	if (engine::InputCommandLinker::GetInstance().IsKeyDown(SDL_SCANCODE_X))
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
#pragma endregion ControlsMenu