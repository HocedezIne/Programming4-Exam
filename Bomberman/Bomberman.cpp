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
#include "GameStateManagingComponent.h"

#include "TextureComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TimerComponent.h"
#include "StatusComponent.h"
#include "UILinkingComponent.h"
#include "ColliderComponent.h"

void load()
{
#if NDEBUG
	engine::ServiceLocator::GetInstance().RegisterSoundSystem(std::make_unique<engine::SoundSystemService>());
#else
	engine::ServiceLocator::RegisterSoundSystem(std::make_unique<engine::LoggingSoundSystemService>(std::make_unique<engine::SoundSystemService>()));
#endif

#pragma region LevelLoading
	auto& levelLoadingScene = engine::sceneManager::CreateScene("Level loading");
	auto text = std::make_unique<engine::GameObject>(glm::vec3{235, 225, 0});
	text->AddComponent(std::make_unique<engine::TextComponent>(text.get(), "Demo level"));
	levelLoadingScene.Add(std::move(text));
#pragma endregion LevelLoading

#pragma region StartMenu
	auto& startScene = engine::sceneManager::CreateScene("Start menu");
	auto logo = std::make_unique<engine::GameObject>(glm::vec3{ 64,0,0 });
	logo->AddComponent(std::make_unique<engine::TextureComponent>(logo.get(), "StartMenu.png"));
	startScene.Add(std::move(logo));

	auto textLine = std::make_unique<engine::GameObject>(glm::vec3{ 144, 304, 0 });
	textLine->AddComponent(std::make_unique<engine::TextComponent>(textLine.get(), "Z START"));
	startScene.Add(std::move(textLine));

	textLine = std::make_unique<engine::GameObject>(glm::vec3{ 320, 304, 0 });
	textLine->AddComponent(std::make_unique<engine::TextComponent>(textLine.get(), "X CONTROLS"));
	startScene.Add(std::move(textLine));

	textLine = std::make_unique<engine::GameObject>(glm::vec3{ 216, 336, 0 });
	textLine->AddComponent(std::make_unique<engine::TextComponent>(textLine.get(), "TOP"));
	startScene.Add(std::move(textLine));

	auto highScore = std::make_unique<engine::GameObject>(glm::vec3{ 416, 336, 0 });
	highScore->AddComponent(std::make_unique<engine::TextComponent>(highScore.get(), "00"));
	startScene.Add(std::move(highScore));

	textLine = std::make_unique<engine::GameObject>(glm::vec3{ 100, 368, 0 });
	textLine->AddComponent(std::make_unique<engine::TextComponent>(textLine.get(), "TM AND C 1987 HUDSON SOFT"));
	startScene.Add(std::move(textLine));

	textLine = std::make_unique<engine::GameObject>(glm::vec3{ 216, 400, 0 });
	textLine->AddComponent(std::make_unique<engine::TextComponent>(textLine.get(), "LICENSED BY"));
	startScene.Add(std::move(textLine));

	textLine = std::make_unique<engine::GameObject>(glm::vec3{ 116, 432, 0 });
	textLine->AddComponent(std::make_unique<engine::TextComponent>(textLine.get(), "NINTENDO OF AMERICA INC."));
	startScene.Add(std::move(textLine));
#pragma endregion StartMenu

#pragma region Controls
	auto& controlsMenu = engine::sceneManager::CreateScene("Controls menu");
	auto fontBig = engine::ResourceManager::GetInstance().LoadFont("nes-arcade-font-monospace.otf", 24);
	auto controls = std::make_unique<engine::GameObject>(glm::vec3{ 200.f, 25.f, 0.f });
	controls->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(controls.get(), "Controls", fontBig));
	controlsMenu.Add(std::move(controls));
	controls = std::make_unique<engine::GameObject>(glm::vec3{ 15.f, 100.f, 0.f });
	controls->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(controls.get(), "Keyboard:"));
	controlsMenu.Add(std::move(controls));
	controls = std::make_unique<engine::GameObject>(glm::vec3{ 25.f, 135.f, 0.f });
	controls->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(controls.get(), "WASD to walk around"));
	controlsMenu.Add(std::move(controls));
	controls = std::make_unique<engine::GameObject>(glm::vec3{ 25.f, 165.f, 0.f });
	controls->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(controls.get(), "C to drop a bomb"));
	controlsMenu.Add(std::move(controls));
#pragma endregion Controls


	auto& LevelScene = engine::sceneManager::CreateScene("Demo level");
	engine::InputCommandLinker::GetInstance().AddKeyboard();

	auto font = engine::ResourceManager::GetInstance().LoadFont("nes-arcade-font-monospace.otf", 16);

#if NDEBUG
#else
	auto fps = std::make_unique<engine::GameObject>(glm::vec3{ 175.f, 20.f, 0.f });
	fps->AddComponent<engine::FPSComponent>(std::make_unique<engine::FPSComponent>(fps.get()));
	LevelScene.Add(std::move(fps));
#endif

#pragma region Level
	auto lvlbg = std::make_unique<engine::GameObject>(glm::vec3{0.f, 100.f,0.f});
	lvlbg->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(lvlbg.get(), "playfield.png"));
	
	// outer bounds
	{
		auto lvlbound = std::make_unique<engine::GameObject>(lvlbg->GetWorldPosition());
		lvlbound->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(lvlbound.get(),
			glm::vec2{ lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().x, 16 }, engine::CollisionMode::Block));
		LevelScene.AddCollidableObject(lvlbound.get());
		LevelScene.Add(std::move(lvlbound));

		lvlbound = std::make_unique<engine::GameObject>(lvlbg->GetWorldPosition());
		lvlbound->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(lvlbound.get(),
			glm::vec2{ 16, lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().y }, engine::CollisionMode::Block));
		LevelScene.AddCollidableObject(lvlbound.get());
		LevelScene.Add(std::move(lvlbound));

		lvlbound = std::make_unique<engine::GameObject>(glm::vec3{ lvlbg->GetWorldPosition().x,
			lvlbg->GetWorldPosition().y + lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().y - 16 ,0.f });
		lvlbound->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(lvlbound.get(),
			glm::vec2{ lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().x, 16 }, engine::CollisionMode::Block));
		LevelScene.AddCollidableObject(lvlbound.get());
		LevelScene.Add(std::move(lvlbound));

		lvlbound = std::make_unique<engine::GameObject>(glm::vec3{ lvlbg->GetWorldPosition().x + lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().x - 16,
			lvlbg->GetWorldPosition().y,0.f });
		lvlbound->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(lvlbound.get(),
			glm::vec2{ 16, lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().y }, engine::CollisionMode::Block));
		LevelScene.AddCollidableObject(lvlbound.get());
		LevelScene.Add(std::move(lvlbound));
	}

	auto bgPos = lvlbg->GetWorldPosition();
	LevelScene.Add(std::move(lvlbg));

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
			LevelScene.AddCollidableObject(lvlbound.get());
			LevelScene.Add(std::move(lvlbound));
		}
	}
#pragma endregion Level

	auto go = std::make_unique<engine::GameObject>(glm::vec3{ 10.f, 20.f, 0.f });
	go->AddComponent<engine::TimerComponent>(std::make_unique<engine::TimerComponent>(go.get(), 200, true));
	LevelScene.Add(std::move(go));

	go = std::make_unique<engine::GameObject>(glm::vec3{ 10.f, 350.f, 0.f });
	go->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(go.get(), "Collisions under construction."));
	LevelScene.Add(std::move(go));
	go = std::make_unique<engine::GameObject>(glm::vec3{ 10.f, 370.f, 0.f });
	go->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(go.get(), "Press R to respawn"));
	LevelScene.Add(std::move(go));


#pragma region playerBomberman
	go = std::make_unique<engine::GameObject>(glm::vec3{ bgPos.x +16, bgPos.y+16, 0.f });
	go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "bomberman.png"));
	go->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(go.get(), go->GetComponent<engine::TextureComponent>()->GetTextureSize(), engine::CollisionMode::Dynamic));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(go.get(), glm::vec3{ 0.f,-1.f,0.f }, 50.f));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(go.get(), glm::vec3{ -1.f,0.f,0.f }, 50.f));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(go.get(), glm::vec3{ 0.f,1.f,0.f }, 50.f));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(go.get(), glm::vec3{ 1.f,0.f,0.f }, 50.f));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>( go.get() ));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_R, engine::KeyState::Pressed, std::make_unique<RespawnBombermanCommand>( go.get() ));

	auto sc = std::make_unique<engine::StatusComponent>(go.get());
	sc->AddDataMapping("LEFT", 3);
	sc->AddDataMapping("SCORE", 0);

	auto goUI = std::make_unique<engine::GameObject>(glm::vec3(500.f, 20.f, 0.f));
	goUI->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(goUI.get(), "", font));
	goUI->AddComponent<engine::UILinkingComponent>(std::make_unique<engine::UILinkingComponent>(goUI.get(), "LEFT", sc.get()));
	LevelScene.Add(std::move(goUI));

	goUI = std::make_unique<engine::GameObject>(glm::vec3(350.f, 20.f, 0.f));
	goUI->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(goUI.get(), "", font));
	goUI->AddComponent<engine::UILinkingComponent>(std::make_unique<engine::UILinkingComponent>(goUI.get(), "SCORE", sc.get(), engine::StringFormat{ 2, '0', false }));
	LevelScene.Add(std::move(goUI));

	go->AddComponent(std::move(sc));
	LevelScene.AddCollidableObject(go.get());
	LevelScene.Add(std::move(go));
#pragma endregion playerBomberman

	go = std::make_unique<engine::GameObject>(glm::vec3(bgPos.x + 16*13, bgPos.y + 16*7, 0.f));
	go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "balloom.png"));
	go->AddComponent<engine::ColliderComponent>(std::make_unique<engine::ColliderComponent>(go.get(), go->GetComponent<engine::TextureComponent>()->GetTextureSize(), engine::CollisionMode::Overlap));
	LevelScene.AddCollidableObject(go.get());
	LevelScene.Add(std::move(go));

	auto& generalScene = engine::sceneManager::CreateScene("");
	auto gameStateHolder = std::make_unique<engine::GameObject>();
	gameStateHolder->AddComponent(std::make_unique<GameStateManagingComponent>(gameStateHolder.get()));
	generalScene.Add(std::move(gameStateHolder));
}

int main(int, char* []) {
	engine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}