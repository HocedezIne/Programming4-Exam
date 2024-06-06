#include  "GameStates.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

#include "DataComponent.h"

GameStateInterface* LevelLostState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
		if (std::any_cast<int>(engine::sceneManager::currentScene->GetObject("player1")->GetComponent<DataComponent>()->GetData("LEFT")) > 0)
			return new LevelLoadingState();
		else
			return new GameOverState();

	return nullptr;
}

void LevelLostState::Update()
{
	m_TimeToStateSwitch -= engine::TimeService::GetInstance().GetDeltaTime();
	if ((m_TimeToStateSwitch - 3.f) < 1e-5f) engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/LevelLost.mp3", true);
}

void LevelLostState::OnEnter()
{
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/BombermanDeath.wav", false);
}

void LevelLostState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}