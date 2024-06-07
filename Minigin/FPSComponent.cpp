#include "FPSComponent.h"

#include <iomanip>
#include <sstream>

#include "GameObject.h"
#include "TimeUtil.h"

namespace engine
{
	void FPSComponent::Update()
	{
		++m_FrameCount;
		m_TotalTime += TimeUtil::GetDeltaTime();

		if (m_TotalTime > 1.f)
		{
			std::ostringstream fpsString;
			fpsString << std::fixed << std::setprecision(1) << (m_FrameCount / m_TotalTime) << " FPS";
			m_TextComp->SetText(fpsString.str());

			m_FrameCount = 0;
			m_TotalTime = 0.f;
		}
	}

	FPSComponent::FPSComponent(GameObject* owner) : Component(owner)
	{
		if (!owner->HasComponent<TextComponent>()) owner->AddComponent<TextComponent>(std::make_unique<TextComponent>(owner, "0 FPS"));
		m_TextComp = owner->GetComponent<TextComponent>();
	}
}