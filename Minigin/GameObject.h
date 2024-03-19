#pragma once

namespace engine
{
	class Texture2D;

	// todo: this should become final.
	class GameObject 
	{
	public:
		virtual void Update();
		virtual void Render() const;

		//void SetTexture(const std::string& filename);
		//void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
	};
}
