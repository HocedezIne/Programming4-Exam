#pragma once

#include "Component.h"
#include "Observer.h"
#include "TextComponent.h"

namespace engine
{
	class TimerComponent : public Component, public IUpdatable, public Subject
	{
	public:
		TimerComponent(GameObject* owner, int duration, bool visualizeTimer = false);
		~TimerComponent() = default;
		TimerComponent(const TimerComponent& other) = delete;
		TimerComponent(TimerComponent&& other) = delete;
		TimerComponent& operator=(const TimerComponent& other) = delete;
		TimerComponent& operator=(TimerComponent&& other) = delete;

		void Update() override;
		void Reset();

	private:
		const int m_Duration;
		float m_CurrentTime;
		
		TextComponent* m_TextComp{ nullptr };
	};
}