#include "Scene.h"
#include <algorithm>
#include <stdexcept>

using namespace engine;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::string name, std::unique_ptr<GameObject> object)
{
	auto it = std::find_if(m_Objects.begin(), m_Objects.end(),
		[&name](const auto& pair) { return pair.first == name; });

	if (it == m_Objects.end())
		m_Objects.emplace_back(name, std::move(object));
	else throw std::invalid_argument("Scene already has object with name " + name + "\n");
}

GameObject* Scene::GetObject(std::string name) const
{
	auto obj = std::find_if(m_Objects.begin(), m_Objects.end(),
		[&name](const auto& pair) { return pair.first == name; });
	if (obj != m_Objects.end()) return obj->second.get();
	else return nullptr;
}

void Scene::Remove(std::string name)
{
	auto obj = std::find_if(m_Objects.begin(), m_Objects.end(),
		[&name](const auto& pair) { return pair.first == name; });
	if (obj != m_Objects.end()) obj->second->MarkDeletion();
}

void Scene::RemoveAll()
{
	m_Objects.clear();
}

void Scene::Update()
{
	if (m_EnableUpdates)
	{
		for (auto& object : m_Objects)
		{
			object.second->Update();
		}
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object.second->Render();
	}
}

void Scene::HandleDeletion()
{
	auto it = m_Objects.begin();
	while (it != m_Objects.end())
	{
		if ((*it).second->IsMarkedForDeletion())
		{
			if (!(*it).second->GetChildren().empty())
			{
				for (auto& child : (*it).second->GetChildren())
				{
					child->SetParent(nullptr, true);
					child->MarkDeletion();
				}
			}
			(*it).second->SetParent(nullptr, true);
			it = m_Objects.erase(it);
		}
		else
		{
			(*it).second->HandleDeletion();
			++it;
		}
	}
}