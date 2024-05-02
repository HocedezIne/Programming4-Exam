#pragma once
#include <string>
#include <memory>
#include <glm/glm.hpp>

#include "Component.h"


namespace engine
{
	class Texture2D;

	class TextureComponent final : public Component, public IRenderable
	{
	public:
		void Render() const override;
		
		void SetTexture(const std::string& fileName);
		void SetTexture(std::shared_ptr<Texture2D> texture);

		const glm::vec2 GetTextureSize() const;

		TextureComponent(GameObject* owner, const std::string& fileName = {});
		~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_Texture {nullptr};
	};
}