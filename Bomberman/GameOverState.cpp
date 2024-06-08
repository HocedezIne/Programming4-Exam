#include "GameStates.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

#include "TextComponent.h"

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
	engine::sceneManager::currentScenes.clear();

	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScenes.push_back(engine::sceneManager::sceneMap["Game over"].get());
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/GameOver.mp3", true);

	if (m_GameMode == GameMode::Vs) engine::sceneManager::currentScenes[0]->GetObject("game over text")->GetComponent<engine::TextComponent>()->SetText("PLAYER 2 WON");
}

void GameOverState::OnExit() 
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();

	engine::sceneManager::currentScenes.clear();
}