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

void Scene::AddCollidableObject(GameObject* object)
{
	m_CollidableObjects.emplace_back(object->GetComponent<ColliderComponent>());
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

	for (int currentIdx{}; currentIdx < int(m_CollidableObjects.size()-1); ++currentIdx)
	{
		for (int checkIdx{ currentIdx + 1 }; checkIdx < int(m_CollidableObjects.size()); ++checkIdx) 
		{
			m_CollidableObjects[currentIdx]->CheckCollision(m_CollidableObjects[checkIdx]);
		}
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