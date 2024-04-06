#include "UILinkingComponent.h"

#include "StatusComponent.h"
#include "GameObject.h"

namespace engine
{
	UILinkingComponent::UILinkingComponent(GameObject* owner, std::string base, StatusComponent* sc) : Component(owner),
		m_Base(base), m_StatusComp(sc)
	{
		if (!owner->HasComponent<TextComponent>()) owner->AddComponent<TextComponent>(std::make_unique<TextComponent>(owner, ""));
		m_TextComp = owner->GetComponent<TextComponent>();

		m_TextComp->SetText(m_Base + ": " + std::to_string(m_StatusComp->GetData(m_Base)));
	}

	void UILinkingComponent::OnNotify(void* caller, Event event, const std::any& args)
	{
		args; caller;

		switch (event)
		{
		case Event::PlayerDied:
		case Event::EnemyDied:
			m_TextComp->SetText(m_Base + ": " + std::to_string(m_StatusComp->GetData(m_Base)));
			break;
		default:
			break;
		}
	}
}