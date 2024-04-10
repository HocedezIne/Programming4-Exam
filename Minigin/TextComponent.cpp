#include "TextComponent.h"

#include <stdexcept>
#include <SDL_ttf.h>

#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "TextureComponent.h"

namespace engine
{
	TextComponent::TextComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font) : Component(owner),
		m_Text(text)
	{
		if (!owner->HasComponent<TextureComponent>()) owner->AddComponent<TextureComponent>(std::make_unique<TextureComponent>(owner));
		m_TextureComp = owner->GetComponent<TextureComponent>();

		if (font != nullptr) m_Font = font;
		else m_Font = ResourceManager::GetInstance().LoadFont("nes-arcade-font-monospace.otf", 16);
	}

	void TextComponent::Update()
	{
		if (m_UpdateTextFlag)
		{
			const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
			const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
			if (surf == nullptr)
			{
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}
			auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
			if (texture == nullptr)
			{
				throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
			}
			SDL_FreeSurface(surf);
			m_TextureComp->SetTexture(std::make_shared<Texture2D>(texture));
			m_UpdateTextFlag = false;
		}
	}

	void TextComponent::SetText(const std::string& text)
	{
		if (m_Text != text)
		{
			m_Text = text;
			m_UpdateTextFlag = true;
		}
	}
}