#include "SceneManager.h"
#include "Scene.h"
#include <stdexcept>

void engine::SceneManager::Update()
{
	currentScene->Update();
}

void engine::SceneManager::Render()
{
	currentScene->Render();
}

void engine::SceneManager::HandleDeletion()
{
	currentScene->HandleDeletion();
}

engine::Scene& engine::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = new Scene(name);
	m_Scenes[name] = scene;
	return *scene;
}

engine::SceneManager::~SceneManager()
{
	for (auto& scenePair : m_Scenes)
	{
		delete scenePair.second;
		scenePair.second = nullptr;
	}
}

void engine::SceneManager::SetCurrentScene(const std::string& name)
{
	if (m_Scenes.find(name) != m_Scenes.end())
	{
		currentScene = m_Scenes[name];
	}
	else
		throw std::invalid_argument("Scene name is unknown\n");
}
