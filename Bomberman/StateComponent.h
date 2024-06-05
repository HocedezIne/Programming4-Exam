#include <memory>

#include "GameStates.h"
#include "GameObject.h"
#include "Component.h"

template <typename State>
class StateComponent : public engine::Component, public engine::IUpdatable
{
public:
	StateComponent(engine::GameObject* owner, State* startingState) : engine::Component(owner)
	{
		m_State = startingState;
		m_State->OnEnter();
	}

	~StateComponent()
	{
		delete m_State;
		m_State = nullptr;
	}

	void Update()
	{
		HandleInput();

		m_State->Update();
	}

private:
	State* m_State{ nullptr };

	void HandleInput()
	{
		auto newState = m_State->HandleInput();
		if (newState != nullptr)
		{
			m_State->OnExit();
			delete m_State;

			m_State = newState;
			m_State->OnEnter();
		}
	}
};