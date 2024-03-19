#include "SceneManager.h"
#include "Scene.h"

void engine::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void engine::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

engine::Scene& engine::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = new Scene(name);
	m_scenes.push_back(scene);
	return *scene;
}

engine::SceneManager::~SceneManager()
{
	for (auto& scene : m_scenes)
	{
		delete scene;
		scene = nullptr;
	}
}
