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

void load()
{
	auto& scene = engine::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_unique<engine::GameObject>();
	go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "background.tga"));
	//go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>("background.tga"));
	scene.Add(std::move(go));

	//auto bg = std::make_shared<dae::TextureComponent>("background.tga");
	//go->AddComponent<dae::TextureComponent>(bg);
	//scene.Add(go);

	//go = std::make_shared<dae::GameObject>();
	//auto logo = std::make_shared<dae::TextureComponent>("logo.tga");
	//logo->SetPosition(216, 180);
	//go->AddComponent<dae::TextureComponent>(logo);
	//scene.Add(go);

	//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto to = std::make_shared<dae::TextComponent>("Programming 4 Assignment", font);
	//to->SetPosition(80, 20);
	//go = std::make_shared<dae::GameObject>();
	//go->AddComponent<dae::TextComponent>(to);
	//scene.Add(go);

	// Testing other GameObject functions 
	//if (go->HasComponent<dae::TextComponent>()) 
	//{ 
	//	auto tc = go->GetComponent<dae::TextComponent>(); 
	//	if (tc) 
	//	{ 
	//		tc->SetText("I was found"); 
	//		go->RemoveComponent<dae::TextComponent>(); 
	//	} 
	//} 
}

int main(int, char*[]) {
	engine::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}