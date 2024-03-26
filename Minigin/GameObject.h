#pragma once
#include <vector>
#include <memory>

namespace engine
{
	class Component;
	class IUpdatable;
	class IRenderable;

	class GameObject final
	{
	public:
		virtual void Update();
		virtual void Render() const;

		template<typename T>
		void AddComponent(std::unique_ptr<T> comp);

		template<typename T>
		void RemoveComponent(T* comp);

		template<typename T>
		T* GetComponent() const;

		template<typename T>
		bool HasComponent() const;

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector< std::unique_ptr<Component> > m_Components{};
		std::vector<IUpdatable*> m_UpdatableComponents{};
		std::vector<IRenderable*> m_RenderableComponents{};
	};
}
