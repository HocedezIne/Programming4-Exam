#include "GameStates.h"
#include "InputCommandLinker.h"
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
}

void LevelState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
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