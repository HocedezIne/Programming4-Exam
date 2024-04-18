#include "TimerComponent.h"

#include <iomanip>
#include <sstream>

#include "GameObject.h"
#include "Time.h"

engine::TimerComponent::TimerComponent(GameObject* owner, int duration, bool visualizeTimer) : Component(owner),
	m_Duration(duration), m_CurrentTime(float(duration))
{
	if (visualizeTimer)
	{
		if (!owner->HasComponent<TextComponent>()) owner->AddComponent<TextComponent>(std::make_unique<TextComponent>(owner, "TIME "));
		m_TextComp = owner->GetComponent<TextComponent>();
	}
}

void engine::TimerComponent::Update()
{
	if (m_CurrentTime > 0.f)
	{
		m_CurrentTime -= Time::GetInstance().GetDeltaTime();
		if (m_CurrentTime <= 0.f)
		{
			NotifyObservers(Event::TimerFinished, this, std::any{});
			m_CurrentTime = 0.f;
		}

		if (m_TextComp)
		{
			std::ostringstream timeString;
			timeString << "TIME " << std::fixed << std::setprecision(0) << std::setw(3) << m_CurrentTime;
			m_TextComp->SetText(timeString.str());
		}
	}
}

void engine::TimerComponent::Reset()
{
	m_CurrentTime = float(m_Duration);
}
