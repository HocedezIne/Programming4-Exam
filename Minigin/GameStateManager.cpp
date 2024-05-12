#include "GameStateManager.h"

namespace engine
{
	void GameStateManager::Init()
	{
		m_CurrentState = new StartMenuState();
		m_CurrentState->OnEnter();
	}

	GameStateManager::~GameStateManager()
	{
		delete m_CurrentState;
		m_CurrentState = nullptr;
	}

	void GameStateManager::HandleInput()
	{
		auto newState = m_CurrentState->HandleInput();

		if (newState != nullptr)
		{
			m_CurrentState->OnExit();
			delete m_CurrentState;

			m_CurrentState = newState;
			m_CurrentState->OnEnter();
		}
	}
}