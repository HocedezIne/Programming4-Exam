#include "GameStateInterface.h"
#include "InputCommandLinker.h"
#include "SceneManager.h"
#include "ServiceLocator.h"
#include "Renderer.h"
#include "TimeService.h"

namespace engine
{
#pragma region StartMenuState
	GameStateInterface* StartMenuState::HandleInput()
	{
		auto& input = InputCommandLinker::GetInstance();

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
		Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
		SceneManager::GetInstance().SetCurrentScene("Start menu");
		ServiceLocator::GetSoundSystem().PlaySound("../Data/StartMenu.mp3", true);
	}

	void StartMenuState::OnExit()
	{
		ServiceLocator::GetSoundSystem().StopAllSound();
	}
#pragma endregion StartMenuState

#pragma region LevelState
	GameStateInterface* LevelState::HandleInput()
	{
		if (InputCommandLinker::GetInstance().IsKeyDown(SDL_SCANCODE_F1))
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
		Renderer::GetInstance().SetBackgroundColor(SDL_Color(173, 173, 173));
		SceneManager::GetInstance().SetCurrentScene("Demo level");
		ServiceLocator::GetSoundSystem().PlaySound("../Data/LevelBackground.mp3", true);
	}

	void LevelState::OnExit()
	{
		ServiceLocator::GetSoundSystem().StopAllSound();
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
		m_TimeToStateSwitch -= TimeService::GetInstance().GetDeltaTime();
	}

	void LevelLoadingState::OnEnter()
	{
		Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
		SceneManager::GetInstance().SetCurrentScene("Level loading");
		ServiceLocator::GetSoundSystem().PlaySound("../Data/LevelStart.mp3", true);
	}

	void LevelLoadingState::OnExit()
	{
		ServiceLocator::GetSoundSystem().StopAllSound();
	}
#pragma endregion LevelLoading

#pragma region ControlsMenu
	GameStateInterface* ControlsMenuState::HandleInput()
	{
		if (InputCommandLinker::GetInstance().IsKeyDown(SDL_SCANCODE_X))
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
		SceneManager::GetInstance().SetCurrentScene("Controls menu");
	}

	void ControlsMenuState::OnExit()
	{
		
	}
#pragma endregion ControlsMenu
}