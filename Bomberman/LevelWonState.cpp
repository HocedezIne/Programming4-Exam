#include "GameStates.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

GameStateInterface* LevelWonState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
	{
		if (m_CurrentLevel == (m_TotalLevels + 1))
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
}