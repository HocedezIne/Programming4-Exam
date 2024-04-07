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
		if (!owner->HasComponent<TextComponent>()) owner->AddComponent<TextComponent>(std::make_unique<TextComponent>(owner, "0 FPS"));
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
			NotifyObservers(this, Event::TimerFinished, std::any{});
			m_CurrentTime = 0.f;
		}

		if (m_TextComp)
		{
			std::ostringstream fpsString;
			fpsString << "TIME " << std::fixed << std::setprecision(0) << m_CurrentTime;
			m_TextComp->SetText(fpsString.str());
		}
	}
}

void engine::TimerComponent::Reset()
{
	m_CurrentTime = float(m_Duration);
}
