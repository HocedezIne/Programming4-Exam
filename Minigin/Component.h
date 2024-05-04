#pragma once

namespace engine
{
	class GameObject;

	class Component {
	public:
		virtual ~Component() = default;

		void MarkDeletion() { m_DeleteFlag = true; };
		bool IsMarkedForDeletion() const { return m_DeleteFlag; };
		
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

	protected:
		Component(GameObject* owner) : m_Owner(owner) {};

		GameObject* GetOwner() const { return m_Owner; };

	private:
		GameObject* m_Owner;
		bool m_DeleteFlag{ false };
	};

	// ------------------------------------------------
	// Interfaces to extend base component with
	// ------------------------------------------------
	class IUpdatable {
	public:
		virtual void Update() = 0;
		virtual ~IUpdatable() = default;
	};

	class IRenderable {
	public:
		virtual void Render() const = 0;
		virtual ~IRenderable() = default;
	};

	class ILateUpdatable {
	public:
		virtual void LateUpdate() = 0;
		virtual ~ILateUpdatable() = default;
	};
}