#include "TextureComponent.h"

#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "TransformComponent.h"

namespace engine
{
	void TextureComponent::Render() const
	{
		if (m_Texture != nullptr)
		{
			const auto& pos = m_TransformComp->GetPosition();
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

	TextureComponent::TextureComponent(GameObject* owner, const std::string& fileName) : Component(owner)
	{
		if (!owner->HasComponent<TransformComponent>()) owner->AddComponent<TransformComponent>(std::make_unique<TransformComponent>(owner));
		m_TransformComp = owner->GetComponent<TransformComponent>();

		if (!fileName.empty()) m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
	}
}