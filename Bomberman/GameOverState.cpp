#include "GameStates.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

GameStateInterface* GameOverState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
		return new HighScoreState();

	return nullptr;
}

void GameOverState::Update()
{
	m_TimeToStateSwitch -= engine::TimeUtil::GetDeltaTime();
}

void GameOverState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScenes.push_back(engine::sceneManager::sceneMap["Game over"].get());
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/GameOver.mp3", true);
}

void GameOverState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();

	engine::sceneManager::currentScenes.clear();
}