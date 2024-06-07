#include "GameStates.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

GameStateInterface* GameWonState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
		return new HighScoreState();

	return nullptr;
}

void GameWonState::Update()
{
	m_TimeToStateSwitch -= engine::TimeUtil::GetDeltaTime();
}

void GameWonState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScene = engine::sceneManager::sceneMap["Game won"].get();
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/GameWon.mp3", true);
}

void GameWonState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}