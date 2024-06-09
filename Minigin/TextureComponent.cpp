#include "TextureComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

namespace engine
{
	void TextureComponent::Render() const
	{
		if (m_Texture != nullptr)
		{
			float width = GetTextureSize().x;
			float height = GetTextureSize().y;

			const auto& pos = GetOwner()->GetWorldPosition();
			Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, width, height);
		}
	}

	void TextureComponent::SetTexture(const std::string& fileName)
	{
		m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
	}

	void TextureComponent::SetTexture(std::shared_ptr<Texture2D> texture)
	{
		m_Texture = texture;
	}

	const glm::vec2 TextureComponent::GetTextureSize() const
	{
		return glm::vec2(m_Texture->GetSize()) * m_Size;
	}

	TextureComponent::TextureComponent(GameObject* owner, const std::string& fileName, const glm::vec2 size) : Component(owner),
		m_Size{size}
	{
		if (!fileName.empty()) m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
	}
}