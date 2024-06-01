#include "GameObject.h"

namespace engine
{
	void GameObject::Update()
	{
		for (const auto& comp : m_UpdatableComponents) comp->Update();
	}

	void GameObject::Render() const
	{
		for (const auto& comp : m_RenderableComponents) comp->Render();
	}

	void GameObject::HandleDeletion()
	{
		auto it = m_Components.begin();
		while (it != m_Components.end())
		{
			if ((*it)->IsMarkedForDeletion())
			{
				if (IUpdatable* ucomp = dynamic_cast<IUpdatable*>(it->get())) 
					m_UpdatableComponents.erase(std::remove(m_UpdatableComponents.begin(), m_UpdatableComponents.end(), (ucomp)), m_UpdatableComponents.end());
				if (IRenderable* rcomp = dynamic_cast<IRenderable*>(it->get())) 
					m_RenderableComponents.erase(std::remove(m_RenderableComponents.begin(), m_RenderableComponents.end(), (rcomp)), m_RenderableComponents.end());

				it = m_Components.erase(it);
		
			}
			else ++it;
		}
	}

	const glm::vec3 GameObject::GetWorldPosition()
	{
		if (m_PositionFlag) UpdateWorldPosition();
		return m_WorldPosition;
	}

	void GameObject::UpdateWorldPosition()
	{
		if (m_PositionFlag)
		{
			if (m_Parent == nullptr) m_WorldPosition = m_LocalPosition;
			else m_WorldPosition = m_Parent->GetWorldPosition() + m_LocalPosition;
		}

		m_PositionFlag = false;
	}

	void GameObject::SetPositionDirty()
	{
		m_PositionFlag = true;

		for (auto& child : m_Children)
		{
			child->SetPositionDirty();
		}
	}

	bool GameObject::IsChild(GameObject* obj) const
	{
		for (const auto& child : m_Children)
		{
			if (obj == child) return true;
			if (child->IsChild(obj)) return true;
		}

		return false;
	}

	void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
	{
		if (IsChild(parent) || parent == this || m_Parent == parent) return;

		if (parent == nullptr) SetLocalPosition(GetWorldPosition());
		else
		{
			if (keepWorldPosition) SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
			SetPositionDirty();
		}

		if (m_Parent) m_Parent->DetachChild(this);
		m_Parent = parent;
		if (m_Parent) m_Parent->AttachChild(this);
	}

	void GameObject::SetLocalPosition(const glm::vec3 pos)
	{
		m_PrevLocalPosition = m_LocalPosition;
		m_LocalPosition = pos;
		m_PositionFlag = true;
	}
}