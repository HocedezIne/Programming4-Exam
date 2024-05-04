#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "Component.h"

namespace engine
{
	class GameObject final
	{
	public:
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render() const;

		void MarkDeletion() { m_DeleteFlag = true; };
		bool IsMarkedForDeletion() const { return m_DeleteFlag; };
		void HandleDeletion();

		void SetLocalPosition(const glm::vec3 pos) { m_LocalPosition = pos; SetPositionDirty(); };
		const glm::vec3 GetLocalPosition() const { return m_LocalPosition; };
		const glm::vec3 GetWorldPosition();
		void UpdateWorldPosition();

		void SetParent(GameObject* parent, bool keepWorldPosition);

		template<typename T>
		void AddComponent(std::unique_ptr<T> comp)
		{
			static_assert(std::is_base_of<Component, T>::value, "Item must derrive from Component class");

			if (auto ucomp = dynamic_cast<IUpdatable*>(comp.get())) m_UpdatableComponents.push_back(ucomp);
			if (auto ucomp = dynamic_cast<ILateUpdatable*>(comp.get())) m_LateUpdatableComponents.push_back(ucomp);
			if (auto rcomp = dynamic_cast<IRenderable*>(comp.get())) m_RenderableComponents.push_back(rcomp);

			m_Components.push_back(std::move(comp));
		};

		template<typename T>
		void RemoveComponent(T* comp) 
		{ 
			for (const auto& ptr : m_Components)
			{
				if (ptr.get() == comp) 
				{
					comp->MarkDeletion();
					break;
				}
			};
		};

		template<typename T>
		T* GetComponent() const
		{
			auto it = std::find_if(m_Components.begin(), m_Components.end(),
				[typeName = typeid(T).name()](const auto& ptr) {
					return ptr && typeid(*ptr) == typeid(T);
				});

			if (it != m_Components.end()) return dynamic_cast<T*>((*it).get());
			return nullptr;
		};

		template<typename T>
		bool HasComponent() const
		{
			auto it = std::find_if(m_Components.begin(), m_Components.end(),
				[typeName = typeid(T).name()](const auto& ptr) {
					return ptr && typeid(*ptr) == typeid(T);
				});

			return it != m_Components.end();
		};

		GameObject(glm::vec3 pos = {}) : m_LocalPosition(pos), m_WorldPosition(m_LocalPosition) {};
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void SetPositionDirty();

		void AttachChild(GameObject* child) { m_Children.push_back(child); };
		void DetachChild(GameObject* child) { m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end()); };
		bool IsChild(GameObject* obj) const;

		std::vector< std::unique_ptr<Component> > m_Components{};
		std::vector<IUpdatable*> m_UpdatableComponents{};
		std::vector<ILateUpdatable*> m_LateUpdatableComponents{};
		std::vector<IRenderable*> m_RenderableComponents{};

		std::vector<GameObject*> m_Children{};
		GameObject* m_Parent{ nullptr };

		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};

		bool m_DeleteFlag{ false };
		bool m_PositionFlag{ false };
	};
}
