#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "InputCommandLinker.h"
#include "GameObject.h"
#include "Scene.h"

#include "TextureComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TimerComponent.h"
#include "StatusComponent.h"
#include "UILinkingComponent.h"

void load()
{
	auto& scene = engine::SceneManager::GetInstance().CreateScene("Demo level");

	//go = std::make_unique<engine::GameObject>();
	//go->AddComponent<engine::TransformComponent>(std::make_unique<engine::TransformComponent>(go.get(), glm::vec3{ 216.f, 180.f, 0.f }));
	//go->AddComponent<engine::FPSComponent>(std::make_unique<engine::FPSComponent>(go.get()));
	//scene.Add(std::move(go));

	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(173, 173, 173));
	engine::InputCommandLinker::GetInstance().AddKeyboard();

	auto font = engine::ResourceManager::GetInstance().LoadFont("nes-arcade-font-monospace.otf", 16);

	auto go = std::make_unique<engine::GameObject>();
	go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "playfield.png"));
	scene.Add(std::move(go));

	go = std::make_unique<engine::GameObject>(glm::vec3{ 10.f, 20.f, 0.f });
	go->AddComponent<engine::TimerComponent>(std::make_unique<engine::TimerComponent>(go.get(), 200, true));
	scene.Add(std::move(go));

#pragma region playerBomberman
	go = std::make_unique<engine::GameObject>(glm::vec3(100.f, 200.f, 0.f));
	go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "bomberman.png"));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(go.get(), glm::vec3{ 0.f,-1.f,0.f }, 50.f));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(go.get(), glm::vec3{ -1.f,0.f,0.f }, 50.f));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(go.get(), glm::vec3{ 0.f,1.f,0.f }, 50.f));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(go.get(), glm::vec3{ 1.f,0.f,0.f }, 50.f));

	auto sc = std::make_unique<engine::StatusComponent>(go.get());
	sc->AddDataMapping("LEFT", 3);
	sc->AddDataMapping("SCORE", 0);

	auto goUI = std::make_unique<engine::GameObject>(glm::vec3(500.f, 20.f, 0.f));
	goUI->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(goUI.get(), "", font));
	goUI->AddComponent<engine::UILinkingComponent>(std::make_unique<engine::UILinkingComponent>(goUI.get(), "LEFT", sc.get()));
	scene.Add(std::move(goUI));

	goUI = std::make_unique<engine::GameObject>(glm::vec3(350.f, 20.f, 0.f));
	goUI->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(goUI.get(), "", font));
	goUI->AddComponent<engine::UILinkingComponent>(std::make_unique<engine::UILinkingComponent>(goUI.get(), "SCORE", sc.get(), engine::StringFormat{ 2, '0', false }));
	scene.Add(std::move(goUI));

	go->AddComponent(std::move(sc));
	scene.Add(std::move(go));
#pragma endregion playerBomberman
}

int main(int, char* []) {
	engine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}