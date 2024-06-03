#pragma once
#include <functional>

#include "Component.h"
#include "TextComponent.h"
#include "Observer.h"

class StatusComponent;

struct StringFormat {
	int width{ 1 };
	char fill{ '0' };
	bool showLabel{ true };
};

class UILinkingComponent : public engine::Component, public engine::IUpdatable, public engine::Observer
{
public:
	UILinkingComponent(engine::GameObject* owner, std::string base, StatusComponent* sc, StringFormat format = {});
	~UILinkingComponent() = default;
	UILinkingComponent(const UILinkingComponent& other) = delete;
	UILinkingComponent(UILinkingComponent&& other) = delete;
	UILinkingComponent& operator=(const UILinkingComponent& other) = delete;
	UILinkingComponent& operator=(UILinkingComponent&& other) = delete;

	virtual void Update() override;
	virtual void OnNotify(engine::Event event, void* caller, const std::any& args) override;

private:
	std::string m_Base;

	engine::TextComponent* m_TextComp;
	StatusComponent* m_StatusComp;

	StringFormat m_Format;

	bool m_UpdateFlag{ true };
};