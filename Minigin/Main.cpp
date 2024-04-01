#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"

#include "TextureComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"

void load()
{
	auto& scene = engine::SceneManager::GetInstance().CreateScene("Demo");

	//go = std::make_unique<engine::GameObject>();
	//go->AddComponent<engine::TransformComponent>(std::make_unique<engine::TransformComponent>(go.get(), glm::vec3{ 216.f, 180.f, 0.f }));
	//go->AddComponent<engine::FPSComponent>(std::make_unique<engine::FPSComponent>(go.get()));
	//scene.Add(std::move(go));

	auto go = std::make_unique<engine::GameObject>();
	go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "background.tga"));
	scene.Add(std::move(go));

	auto font = engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_unique<engine::GameObject>(glm::vec3{80.f, 20.f, 0.f});
	go->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(go.get(), "Prog 4 Exam engine", font));
	scene.Add(std::move(go));
}

int main(int, char*[]) {
	engine::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}