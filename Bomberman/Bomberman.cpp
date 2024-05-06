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
#include "BombermanCommands.h"
#include "GameObject.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "SoundSystemService.h"

#include "TextureComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TimerComponent.h"
#include "StatusComponent.h"
#include "UILinkingComponent.h"
#include "ColliderComponent.h"

void load()
{
	auto& scene = engine::SceneManager::GetInstance().CreateScene("Demo level");

	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(173, 173, 173));
	engine::InputCommandLinker::GetInstance().AddKeyboard();
	
#if NDEBUG
	engine::ServiceLocator::GetInstance().RegisterSoundSystem(std::make_unique<engine::SoundSystemService>());
#else
	engine::ServiceLocator::RegisterSoundSystem(std::make_unique<engine::LoggingSoundSystemService>(std::make_unique<engine::SoundSystemService>()));

	auto fps = std::make_unique<engine::GameObject>(glm::vec3{ 175.f, 20.f, 0.f });
	fps->AddComponent<engine::FPSComponent>(std::make_unique<engine::FPSComponent>(fps.get()));
	scene.Add(std::move(fps));
#endif

	auto font = engine::ResourceManager::GetInstance().LoadFont("nes-arcade-font-monospace.otf", 16);

#pragma region Level
	auto lvlbg = std::make_unique<engine::GameObject>(glm::vec3{0.f, 100.f,0.f});
	lvlbg->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(lvlbg.get(), "playfield.png"));
	
	// outer bounds
	{
		auto lvlbound = std::make_unique<engine::GameObject>(lvlbg->GetWorldPosition());
		lvlbound->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(lvlbound.get(),
			glm::vec2{ lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().x, 16 }, engine::CollisionMode::Block));
		scene.AddCollidableObject(lvlbound.get());
		scene.Add(std::move(lvlbound));

		lvlbound = std::make_unique<engine::GameObject>(lvlbg->GetWorldPosition());
		lvlbound->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(lvlbound.get(),
			glm::vec2{ 16, lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().y }, engine::CollisionMode::Block));
		scene.AddCollidableObject(lvlbound.get());
		scene.Add(std::move(lvlbound));

		lvlbound = std::make_unique<engine::GameObject>(glm::vec3{ lvlbg->GetWorldPosition().x,
			lvlbg->GetWorldPosition().y + lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().y - 16 ,0.f });
		lvlbound->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(lvlbound.get(),
			glm::vec2{ lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().x, 16 }, engine::CollisionMode::Block));
		scene.AddCollidableObject(lvlbound.get());
		scene.Add(std::move(lvlbound));

		lvlbound = std::make_unique<engine::GameObject>(glm::vec3{ lvlbg->GetWorldPosition().x + lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().x - 16,
			lvlbg->GetWorldPosition().y,0.f });
		lvlbound->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(lvlbound.get(),
			glm::vec2{ 16, lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().y }, engine::CollisionMode::Block));
		scene.AddCollidableObject(lvlbound.get());
		scene.Add(std::move(lvlbound));
	}

	auto bgPos = lvlbg->GetWorldPosition();
	scene.Add(std::move(lvlbg));

	// inner blocks
	for (int r{}; r < 5; ++r)
	{
		glm::vec3 pos{ };
		pos.y = bgPos.y + (r + 1) * 32;
		for (int c{}; c < 14; ++c)
		{
			pos.x = bgPos.x + (c+1) * 32;
			auto lvlbound = std::make_unique<engine::GameObject>(pos);
			lvlbound->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(lvlbound.get(), glm::vec2{ 16, 16 }, engine::CollisionMode::Block));
			scene.AddCollidableObject(lvlbound.get());
			scene.Add(std::move(lvlbound));
		}
	}
#pragma endregion Level

	auto go = std::make_unique<engine::GameObject>(glm::vec3{ 10.f, 20.f, 0.f });
	go->AddComponent<engine::TimerComponent>(std::make_unique<engine::TimerComponent>(go.get(), 200, true));
	scene.Add(std::move(go));

	go = std::make_unique<engine::GameObject>(glm::vec3{10.f, 50.f, 0.f});
	go->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(go.get(), "WASD to walk around"));
	scene.Add(std::move(go));
	go = std::make_unique<engine::GameObject>(glm::vec3{ 10.f, 70.f, 0.f });
	go->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(go.get(), "C to drop a bomb"));
	scene.Add(std::move(go));

#pragma region playerBomberman
	go = std::make_unique<engine::GameObject>(glm::vec3{ bgPos.x +16, bgPos.y+16, 0.f });
	go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "bomberman.png"));
	go->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(go.get(), go->GetComponent<engine::TextureComponent>()->GetTextureSize(), engine::CollisionMode::Dynamic));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(go.get(), glm::vec3{ 0.f,-1.f,0.f }, 50.f));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(go.get(), glm::vec3{ -1.f,0.f,0.f }, 50.f));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(go.get(), glm::vec3{ 0.f,1.f,0.f }, 50.f));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(go.get(), glm::vec3{ 1.f,0.f,0.f }, 50.f));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>( go.get() ));

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
	scene.AddCollidableObject(go.get());
	scene.Add(std::move(go));
#pragma endregion playerBomberman

	//go = std::make_unique<engine::GameObject>(glm::vec3(200.f, 200.f, 0.f));
	//go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "balloom.png"));
	//go->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(go.get(), go->GetComponent<engine::TextureComponent>()->GetTextureSize(), engine::CollisionMode::Overlap));
	//scene.AddCollidableObject(go.get());
	//scene.Add(std::move(go));

	auto& ss = engine::ServiceLocator::GetSoundSystem();
	ss.PlaySound("../Data/LevelBackground.mp3", true);
}

int main(int, char* []) {
	engine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}