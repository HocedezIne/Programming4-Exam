#pragma once
#include <functional>

#include "Component.h"
#include "TextComponent.h"
#include "Observer.h"

namespace engine
{
	class StatusComponent;

	struct StringFormat {
		int width{ 1 };
		char fill{ '0' };
		bool showLabel{ true };
	};

	class UILinkingComponent : public Component, public Observer
	{
	public:
		UILinkingComponent(GameObject* owner, std::string base, StatusComponent* sc, StringFormat format = {});
		~UILinkingComponent() = default;
		UILinkingComponent(const UILinkingComponent& other) = delete;
		UILinkingComponent(UILinkingComponent&& other) = delete;
		UILinkingComponent& operator=(const UILinkingComponent& other) = delete;
		UILinkingComponent& operator=(UILinkingComponent&& other) = delete;

		virtual void OnNotify(void* caller, Event event, const std::any& args) override;

	private:
		void ModifyText();

		std::string m_Base;

		TextComponent* m_TextComp;
		StatusComponent* m_StatusComp;

		StringFormat m_Format;
	};
}