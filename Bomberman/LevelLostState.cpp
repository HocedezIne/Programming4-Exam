#include  "GameStates.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

#include "BombControllerComponent.h"
#include "DataComponent.h"

GameStateInterface* LevelLostState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
		if (std::any_cast<int>(engine::sceneManager::currentScenes[0]->GetObject("player1")->GetComponent<DataComponent>()->GetData("LEFT")) > 0)
			return new LevelLoadingState();
		else
			return new GameOverState();

	return nullptr;
}

void LevelLostState::Update()
{
	m_TimeToStateSwitch -= engine::TimeUtil::GetDeltaTime();
	if ((m_TimeToStateSwitch - 3.f) < 1e-5f && m_PlayMusic)
	{
		m_PlayMusic = false;
		engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/LevelLost.mp3", true);
	}
}

void LevelLostState::OnEnter()
{
	m_PlayMusic = true;
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/BombermanDeath.wav", false);

	auto scene = engine::sceneManager::currentScenes[0];
	switch (m_GameMode)
	{
	case GameMode::Coop:
		scene->GetObject("player2")->GetComponent<BombControllerComponent>()->ResetRemote();

	case GameMode::Vs:
	case GameMode::Single:
		scene->GetObject("player1")->GetComponent<BombControllerComponent>()->ResetRemote();
		break;
	default:
		break;
	}
}

void LevelLostState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
	engine::sceneManager::currentScenes.clear();
}