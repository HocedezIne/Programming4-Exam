#include "GameObject.h"

#include "Component.h"

namespace engine
{
	GameObject::~GameObject() = default;

	void GameObject::Update()
	{
		for (const auto& comp : m_UpdatableComponents) comp->Update();
	}

	void GameObject::Render() const
	{
		for (const auto& comp : m_RenderableComponents) comp->Render();
	}

	template<typename T>
	void GameObject::AddComponent(std::unique_ptr<T> comp)
	{
		static_assert(std::is_base_of<Component, T>::value, "Item must derrive from Component class");

		if (auto ucomp = std::dynamic_pointer_cast<IUpdatable>(comp)) m_UpdatableComponents.push_back(ucomp.get());
		if (auto rcomp = std::dynamic_pointer_cast<IRenderable>(comp)) m_RenderableComponents.push_back(rcomp.get());

		m_Components.push_back(std::move(comp));
	};

	template<typename T>
	void GameObject::RemoveComponent(T* comp)
	{

	}

	template<typename T>
	T* GameObject::GetComponent() const
	{
		auto it = std::find_if(m_Components.begin(), m_Components.end(),
			[typeName = typeid(T).name()](const auto& ptr) {
				return ptr && typeid(*ptr) == typeid(T);
			});

		if (it != m_Components.end()) return (*it).get();
	}

	template<typename T>
	bool GameObject::HasComponent() const
	{
		auto it = std::find_if(m_Components.begin(), m_Components.end(),
			[typeName = typeid(T).name()](const auto& ptr) {
				return ptr && typeid(*ptr) == typeid(T);
			});

		return it != m_Components.end();
	}
}