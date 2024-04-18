#include "UILinkingComponent.h"

#include <iomanip>
#include <sstream>

#include "StatusComponent.h"
#include "GameObject.h"

namespace engine
{
	UILinkingComponent::UILinkingComponent(GameObject* owner, std::string base, StatusComponent* sc, StringFormat format) : Component(owner),
		m_Base(base), m_StatusComp(sc), m_Format(format)
	{
		if (!owner->HasComponent<TextComponent>()) owner->AddComponent<TextComponent>(std::make_unique<TextComponent>(owner, ""));
		m_TextComp = owner->GetComponent<TextComponent>();

		ModifyText();
	}

	void UILinkingComponent::OnNotify(Event event, void* /*caller*/, const std::any& /*args*/)
	{
		switch (event)
		{
		case Event::PlayerDied:
		case Event::EnemyDied:
			ModifyText();
			break;
		default:
			break;
		}
	}

	void UILinkingComponent::ModifyText()
	{
		std::string dataString{ std::to_string(m_StatusComp->GetData(m_Base)) };

		std::ostringstream dataFormatString;
		if (m_Format.showLabel) dataFormatString << m_Base << " ";
		dataFormatString << std::setw(m_Format.width) << std::setfill(m_Format.fill) << dataString;

		m_TextComp->SetText(dataFormatString.str());
	}
}