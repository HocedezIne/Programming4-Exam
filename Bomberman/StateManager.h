#pragma once
#include "Singleton.h"

template <class State>
class StateManager : public engine::Singleton< StateManager<State> >
{
public:
	virtual ~StateManager()
	{
		delete m_CurrentState;
		m_CurrentState = nullptr;
	}

	virtual void Init() = 0;

	void Update() { m_CurrentState->Update(); };
	virtual void HandleInput() = 0;

	State* GetCurrentState() const { return m_CurrentState; };

protected:
	State* m_CurrentState{ nullptr };

private:
	StateManager() = default;
};