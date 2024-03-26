#include "GameObject.h"

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
}