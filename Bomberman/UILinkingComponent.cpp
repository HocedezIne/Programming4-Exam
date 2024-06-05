#include "UILinkingComponent.h"

#include <iomanip>
#include <sstream>

#include "DataComponent.h"
#include "GameObject.h"

UILinkingComponent::UILinkingComponent(engine::GameObject* owner, std::string base, DataComponent* sc, StringFormat format) : 
	engine::Component(owner),
	m_Base(base), m_StatusComp(sc), m_Format(format)
{
	if (!owner->HasComponent<engine::TextComponent>()) 
		owner->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(owner, ""));
	m_TextComp = owner->GetComponent<engine::TextComponent>();

	Update();
}

void UILinkingComponent::OnNotify(engine::Event event, void* /*caller*/, const std::any& /*args*/)
{
	switch (event)
	{
	case engine::Event::PlayerDied:
	case engine::Event::EnemyDied:
		m_UpdateFlag = true;
		break;
	default:
		break;
	}
}

void UILinkingComponent::Update()
{
	if (m_UpdateFlag)
	{
		std::string dataString{ std::to_string(std::any_cast<int>(m_StatusComp->GetData(m_Base))) };

		std::ostringstream dataFormatString;
		if (m_Format.showLabel) dataFormatString << m_Base << " ";
		dataFormatString << std::setw(m_Format.width) << std::setfill(m_Format.fill) << dataString;

		m_TextComp->SetText(dataFormatString.str());
	}
}