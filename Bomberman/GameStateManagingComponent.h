#include <memory>

#include "GameStates.h"
#include "GameObject.h"
#include "Component.h"

class GameStateManagingComponent : public engine::Component, public engine::IUpdatable
{
public:
	GameStateManagingComponent(engine::GameObject* owner) : engine::Component(owner)
	{
		m_GameState = new StartMenuState();
		m_GameState->OnEnter();
	}

	~GameStateManagingComponent()
	{
		delete m_GameState;
		m_GameState = nullptr;
	}

	void Update()
	{
		HandleInput();

		m_GameState->Update();
	}

private:
	GameStateInterface* m_GameState{ nullptr };

	void HandleInput()
	{
		auto newState = m_GameState->HandleInput();
		if (newState != nullptr)
		{
			m_GameState->OnExit();
			delete m_GameState;

			m_GameState = newState;
			m_GameState->OnEnter();
		}
	}
};