#include "GameStates.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

#include "DataComponent.h"

GameStateInterface* LevelWonState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
	{
		if (m_GameMode == GameMode::Vs)
		{
			if (std::any_cast<int>(engine::sceneManager::currentScenes[0]->GetObject("player1")->GetComponent<DataComponent>()->GetData("LEFT")) > 0 &&
				std::any_cast<int>(engine::sceneManager::currentScenes[0]->GetObject("player2")->GetComponent<DataComponent>()->GetData("LEFT")) > 0)
				return new LevelLoadingState();
			else 
				return new GameWonState();
		}

		else if (m_CurrentLevel == (m_TotalLevels + 1))
			return new GameWonState();
		else
			return new LevelLoadingState();
	}


	return nullptr;
}

void LevelWonState::Update()
{
	m_TimeToStateSwitch -= engine::TimeUtil::GetDeltaTime();
}

void LevelWonState::OnEnter()
{
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/LevelFinished.mp3", true);
	++m_CurrentLevel;
}

void LevelWonState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
	engine::sceneManager::currentScenes.clear();
}