#include "GameStates.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

#include "TextComponent.h"

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
	engine::sceneManager::currentScenes.clear();

	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScenes.push_back(engine::sceneManager::sceneMap["Game won"].get());
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/GameWon.mp3", true);

	if (m_GameMode == GameMode::Vs) engine::sceneManager::currentScenes[0]->GetObject("game won text")->GetComponent<engine::TextComponent>()->SetText("PLAYER 1 WON");
}

void GameWonState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();

	engine::sceneManager::currentScenes.clear();
}