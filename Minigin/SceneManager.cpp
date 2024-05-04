#include "SceneManager.h"
#include "Scene.h"

void engine::SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void engine::SceneManager::LateUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->LateUpdate();
	}
}

void engine::SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

void engine::SceneManager::HandleDeletion()
{
	for (const auto& scene : m_Scenes) scene->HandleDeletion();
}

engine::Scene& engine::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = new Scene(name);
	m_Scenes.push_back(scene);
	return *scene;
}

engine::SceneManager::~SceneManager()
{
	for (auto& scene : m_Scenes)
	{
		delete scene;
		scene = nullptr;
	}
}
