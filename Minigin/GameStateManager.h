#pragma once
#include "Singleton.h"
#include "GameStateInterface.h"

namespace engine
{
	class GameStateManager final : public Singleton<GameStateManager>
	{
	public:
		virtual ~GameStateManager();

		void Init();

		void Update() { m_CurrentState->Update(); };
		void HandleInput();

		GameStateInterface* GetCurrentState() const { return m_CurrentState; };

	private:
		GameStateInterface* m_CurrentState{nullptr};
	};
}