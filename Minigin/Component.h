#pragma once

#include "GameObject.h"

namespace engine
{
	class Component {
	public:
		virtual ~Component() = default;
		
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		Component(GameObject* owner) : m_Owner(owner) {};

		const GameObject* GetOwner() const { return m_Owner; };

	private:
		const GameObject* m_Owner;
	};

	// ------------------------------------------------
	// Interfaces to extend base component with
	// ------------------------------------------------
	class IUpdatable {
	public:
		virtual void Update() = 0;
	};

	class IRenderable {
	public:
		virtual void Render() const = 0;
	};
}