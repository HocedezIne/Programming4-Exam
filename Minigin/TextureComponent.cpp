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
			const auto& pos = GetOwner()->GetWorldPosition();
			Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
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
		return m_Texture->GetSize();
	}

	TextureComponent::TextureComponent(GameObject* owner, const std::string& fileName) : Component(owner)
	{
		if (!fileName.empty()) m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
	}
}