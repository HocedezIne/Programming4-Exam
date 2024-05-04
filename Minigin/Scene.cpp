#include "Scene.h"
#include <algorithm>

using namespace engine;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	m_Objects.emplace_back(std::move(object));
}

void Scene::Remove(std::unique_ptr<GameObject> object)
{
	object->MarkDeletion();
}

void Scene::RemoveAll()
{
	m_Objects.clear();
}

void Scene::Update()
{
	for(auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::LateUpdate()
{
	for (auto& object : m_Objects)
	{
		object->LateUpdate();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

void Scene::HandleDeletion()
{
	auto it = m_Objects.begin();
	while (it != m_Objects.end())
	{
		if ((*it)->IsMarkedForDeletion()) it = m_Objects.erase(it);
		else
		{
			(*it)->HandleDeletion();
			++it;
		}
	}
}