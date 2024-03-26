#pragma once
#include <string>
#include <memory>

#include "Component.h"

namespace engine 
{
	class Font;
	class TextureComponent;

	class TextComponent final: public Component, public IUpdatable
	{
	public:
		void Update() override;
		void SetText(const std::string& text);

		TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font = nullptr);
		~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

	private:
		bool m_UpdateTextFlag{true};
		std::string m_Text;
		std::shared_ptr<Font> m_Font;

		TextureComponent* m_TextureComp;
	};
}