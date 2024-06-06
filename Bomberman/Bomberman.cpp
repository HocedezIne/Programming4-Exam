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
#include "ServiceLocator.h"
#include "SoundSystemService.h"
#include "StateComponent.h"
#include "CollisionSystem.h"
#include "EnemyController.h"

#include "TextureComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "TimerComponent.h"
#include "DataComponent.h"
#include "UILinkingComponent.h"
#include "ColliderComponent.h"

#include "HighscoreData.h"

void load()
{
#if NDEBUG
	engine::ServiceLocator::GetInstance().RegisterSoundSystem(std::make_unique<engine::SoundSystemService>());
#else
	engine::ServiceLocator::RegisterSoundSystem(std::make_unique<engine::LoggingSoundSystemService>(std::make_unique<engine::SoundSystemService>()));
#endif

#pragma region LevelLoading
	auto& levelLoadingScene = engine::sceneManager::CreateScene("Level loading");
	auto text = std::make_unique<engine::GameObject>(glm::vec3{235.f, 225.f, 0.f});
	text->AddComponent(std::make_unique<engine::TextComponent>(text.get(), "Demo level"));
	levelLoadingScene.Add("level text",std::move(text));
#pragma endregion LevelLoading

#pragma region GameOver
	auto& gameOverScene = engine::sceneManager::CreateScene("Game over");
	text = std::make_unique<engine::GameObject>(glm::vec3{ 235.f, 225.f, 0.f });
	text->AddComponent(std::make_unique<engine::TextComponent>(text.get(), "GAME OVER"));
	gameOverScene.Add("game over text", std::move(text));
#pragma endregion GameOver

#pragma region GameWon
	auto& gameWonScene = engine::sceneManager::CreateScene("Game won");
	text = std::make_unique<engine::GameObject>(glm::vec3{ 200.f, 225.f, 0.f });
	text->AddComponent(std::make_unique<engine::TextComponent>(text.get(), "GAME COMPLETE"));
	gameWonScene.Add("game won text", std::move(text));
#pragma endregion GameWon

#pragma region StartMenu
	auto& startScene = engine::sceneManager::CreateScene("Start menu");
	auto logo = std::make_unique<engine::GameObject>(glm::vec3{ 64.f,0.f,0.f });
	logo->AddComponent(std::make_unique<engine::TextureComponent>(logo.get(), "StartMenu.png"));
	startScene.Add("logo", std::move(logo));

	auto cursor = std::make_unique<engine::GameObject>(glm::vec3{ 144.f, 304.f, 0.f });
	cursor->AddComponent(std::make_unique<engine::TextComponent>(cursor.get(), ">"));
	startScene.Add("cursor", std::move(cursor));

	auto textLine = std::make_unique<engine::GameObject>(glm::vec3{ 160.f, 304.f, 0.f });
	textLine->AddComponent(std::make_unique<engine::TextComponent>(textLine.get(), "START"));
	startScene.Add("start", std::move(textLine));

	textLine = std::make_unique<engine::GameObject>(glm::vec3{ 336.f, 304.f, 0.f });
	textLine->AddComponent(std::make_unique<engine::TextComponent>(textLine.get(), "CONTROLS"));
	startScene.Add("controls", std::move(textLine));

	textLine = std::make_unique<engine::GameObject>(glm::vec3{ 216.f, 336.f, 0.f });
	textLine->AddComponent(std::make_unique<engine::TextComponent>(textLine.get(), "TOP"));
	startScene.Add("top", std::move(textLine));

	auto highScore = std::make_unique<engine::GameObject>(glm::vec3{ 416.f, 336.f, 0.f });
	highScore->AddComponent(std::make_unique<engine::TextComponent>(highScore.get(), highscoreData::GetHighscoreString() ));
	startScene.Add("score", std::move(highScore));

	textLine = std::make_unique<engine::GameObject>(glm::vec3{ 100.f, 368.f, 0.f });
	textLine->AddComponent(std::make_unique<engine::TextComponent>(textLine.get(), "TM AND C 1987 HUDSON SOFT"));
	startScene.Add("copywright", std::move(textLine));

	textLine = std::make_unique<engine::GameObject>(glm::vec3{ 216.f, 400.f, 0.f });
	textLine->AddComponent(std::make_unique<engine::TextComponent>(textLine.get(), "LICENSED BY"));
	startScene.Add("license", std::move(textLine));

	textLine = std::make_unique<engine::GameObject>(glm::vec3{ 116.f, 432.f, 0.f });
	textLine->AddComponent(std::make_unique<engine::TextComponent>(textLine.get(), "NINTENDO OF AMERICA INC."));
	startScene.Add("nintendo", std::move(textLine));
#pragma endregion StartMenu

#pragma region Controls
	auto& controlsMenu = engine::sceneManager::CreateScene("Controls menu");
	auto fontBig = engine::ResourceManager::GetInstance().LoadFont("nes-arcade-font-monospace.otf", 24);
	auto controls = std::make_unique<engine::GameObject>(glm::vec3{ 200.f, 25.f, 0.f });
	controls->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(controls.get(), "Controls", fontBig));
	controlsMenu.Add("controls", std::move(controls));
	controls = std::make_unique<engine::GameObject>(glm::vec3{ 15.f, 100.f, 0.f });
	controls->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(controls.get(), "Keyboard:"));
	controlsMenu.Add("keyboard", std::move(controls));
	controls = std::make_unique<engine::GameObject>(glm::vec3{ 25.f, 135.f, 0.f });
	controls->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(controls.get(), "WASD to walk around"));
	controlsMenu.Add("wasd", std::move(controls));
	controls = std::make_unique<engine::GameObject>(glm::vec3{ 25.f, 165.f, 0.f });
	controls->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(controls.get(), "C to drop a bomb"));
	controlsMenu.Add("c", std::move(controls));
#pragma endregion Controls

#pragma region HighScoreMenu
	auto& highscoreScene = engine::sceneManager::CreateScene("Highscore menu");

	auto title = std::make_unique<engine::GameObject>(glm::vec3{ 225.f, 50.f,0.f });
	title->AddComponent(std::make_unique<engine::TextComponent>(title.get(), "HIGHSCORES"));
	highscoreScene.Add("highscores", std::move(title));

	auto player1text = std::make_unique<engine::GameObject>(glm::vec3{ 150.f, 82.f,0.f });
	player1text->AddComponent(std::make_unique<engine::TextComponent>(player1text.get(), "PLAYER 1 SCORE"));
	highscoreScene.Add("player1text", std::move(player1text));

	auto player2text = std::make_unique<engine::GameObject>(glm::vec3{ 150.f, 106.f,0.f });
	player2text->AddComponent(std::make_unique<engine::TextComponent>(player2text.get(), "PLAYER 2 SCORE"));
	highscoreScene.Add("player2text", std::move(player2text));

	auto rank = std::make_unique<engine::GameObject>(glm::vec3{ 100.f, 138.f,0.f });
	rank->AddComponent(std::make_unique<engine::TextComponent>(rank.get(), "RANK"));
	highscoreScene.Add("rank", std::move(rank));

	auto score = std::make_unique<engine::GameObject>(glm::vec3{ 300.f, 138.f,0.f });
	score->AddComponent(std::make_unique<engine::TextComponent>(score.get(), "SCORE"));
	highscoreScene.Add("score", std::move(score));

	auto name = std::make_unique<engine::GameObject>(glm::vec3{ 500.f, 138.f,0.f });
	name->AddComponent(std::make_unique<engine::TextComponent>(name.get(), "NAME"));
	highscoreScene.Add("name", std::move(name));
#pragma endregion HighScoreMenu

#pragma region Level
	auto& LevelScene = engine::sceneManager::CreateScene("Demo level");
	engine::InputCommandLinker::GetInstance().AddKeyboard();

	auto font = engine::ResourceManager::GetInstance().LoadFont("nes-arcade-font-monospace.otf", 16);

#if NDEBUG
#else
	auto fps = std::make_unique<engine::GameObject>(glm::vec3{ 175.f, 20.f, 0.f });
	fps->AddComponent<engine::FPSComponent>(std::make_unique<engine::FPSComponent>(fps.get()));
	LevelScene.Add("fps", std::move(fps));
#endif

	auto go = std::make_unique<engine::GameObject>(glm::vec3{ 10.f, 20.f, 0.f });
	go->AddComponent<engine::TimerComponent>(std::make_unique<engine::TimerComponent>(go.get(), 200, true));
	LevelScene.Add("timer", std::move(go));

	go = std::make_unique<engine::GameObject>(glm::vec3{ 10.f, 350.f, 0.f });
	go->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(go.get(), "Collisions under construction."));
	LevelScene.Add("collision warning", std::move(go));
	go = std::make_unique<engine::GameObject>(glm::vec3{ 10.f, 370.f, 0.f });
	go->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(go.get(), "Press R to respawn"));
	LevelScene.Add("respawn", std::move(go));

	auto lvlbg = std::make_unique<engine::GameObject>(glm::vec3{0.f, 96.f,0.f});
	lvlbg->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(lvlbg.get(), "playfield.png"));
	
	// outer bounds
	{
		auto lvlbound = std::make_unique<engine::GameObject>();
		lvlbound->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(lvlbound.get(),
			glm::vec2{ lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().x, 16.f }, true, CollisionType::Wall));
		lvlbound->SetParent(lvlbg.get(), false);
		LevelScene.Add("top bound", std::move(lvlbound));

		lvlbound = std::make_unique<engine::GameObject>();
		lvlbound->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(lvlbound.get(),
			glm::vec2{ 16.f, lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().y }, true, CollisionType::Wall));
		lvlbound->SetParent(lvlbg.get(), false);
		LevelScene.Add("left bound", std::move(lvlbound));

		lvlbound = std::make_unique<engine::GameObject>(glm::vec3{ 0.f, lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().y - 16.f ,0.f });
		lvlbound->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(lvlbound.get(),
			glm::vec2{ lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().x, 16.f }, true, CollisionType::Wall));
		lvlbound->SetParent(lvlbg.get(), false);
		LevelScene.Add("bottom bound", std::move(lvlbound));

		lvlbound = std::make_unique<engine::GameObject>(glm::vec3{ lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().x - 16.f,0.f,0.f });
		lvlbound->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(lvlbound.get(),
			glm::vec2{ 16.f, lvlbg->GetComponent<engine::TextureComponent>()->GetTextureSize().y }, true, CollisionType::Wall));
		lvlbound->SetParent(lvlbg.get(), false);
		LevelScene.Add("right bound", std::move(lvlbound));
	}

	auto bgptr = lvlbg.get();
	LevelScene.Add("bg", std::move(lvlbg));

	// inner blocks
	for (int r{}; r < 5; ++r)
	{
		glm::vec3 pos{ };
		pos.y = (r + 1) * 32.f;
		for (int c{}; c < 14; ++c)
		{
			pos.x = (c+1) * 32.f;
			auto lvlbound = std::make_unique<engine::GameObject>(pos);
			lvlbound->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(lvlbound.get(), 
				glm::vec2{ 16.f, 16.f }, true, CollisionType::Block));
			lvlbound->SetParent(bgptr, false);
			LevelScene.Add("r" + std::to_string(r + 1) + "c" + std::to_string(c + 1), std::move(lvlbound));
		}
	}
#pragma endregion Level

#pragma region playerBomberman
	go = std::make_unique<engine::GameObject>(glm::vec3{ 16, 16, 0.f });
	go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "bomberman.png"));
	go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(), 
		go->GetComponent<engine::TextureComponent>()->GetTextureSize(), false, CollisionType::Player));
	
	auto sc = std::make_unique<DataComponent>(go.get());
	sc->AddDataMapping("LEFT", 3);
	sc->AddDataMapping("SCORE", 0);
	collisionSystem::collisionHandler.AddObserver(sc.get());
	enemyController::EnemyController::GetInstance().AddObserver(sc.get());

	auto goUI = std::make_unique<engine::GameObject>(glm::vec3(500.f, 20.f, 0.f));
	goUI->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(goUI.get(), "", font));
	goUI->AddComponent<UILinkingComponent>(std::make_unique<UILinkingComponent>(goUI.get(), "LEFT", sc.get()));
	collisionSystem::collisionHandler.AddObserver(goUI->GetComponent<UILinkingComponent>());
	LevelScene.Add("player1 lives", std::move(goUI));

	goUI = std::make_unique<engine::GameObject>(glm::vec3(350.f, 20.f, 0.f));
	goUI->AddComponent<engine::TextComponent>(std::make_unique<engine::TextComponent>(goUI.get(), "", font));
	goUI->AddComponent<UILinkingComponent>(std::make_unique<UILinkingComponent>(goUI.get(), "SCORE", sc.get(), StringFormat{ 2, '0', false }));
	enemyController::EnemyController::GetInstance().AddObserver(goUI->GetComponent<UILinkingComponent>());
	LevelScene.Add("player1 score", std::move(goUI));

	go->AddComponent<DataComponent>(std::move(sc));
	go->SetParent(bgptr, false);
	LevelScene.Add("player1", std::move(go));
#pragma endregion playerBomberman

	enemyController::EnemyController::GetInstance().AddBalloomEnemy(13, 7);
	enemyController::EnemyController::GetInstance().AddBalloomEnemy(5, 11);
	enemyController::EnemyController::GetInstance().AddDollEnemy(3, 9);

	go = std::make_unique<engine::GameObject>(glm::vec3(16.f * 6, 16.f * 3, 0.f));
	go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "destructible.png"));
	go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
		go->GetComponent<engine::TextureComponent>()->GetTextureSize(), true, CollisionType::Destructable));
	go->SetParent(bgptr, false);
	LevelScene.Add("destructible1", std::move(go));

	go = std::make_unique<engine::GameObject>(glm::vec3(16.f * 10, 16.f * 5, 0.f));
	go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "door.png"));
	go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
		go->GetComponent<engine::TextureComponent>()->GetTextureSize(), true, CollisionType::Door));
	sc = std::make_unique<DataComponent>(go.get());
	sc->AddDataMapping("BLOCKED", true);
	go->AddComponent<DataComponent>(std::move(sc));
	go->SetParent(bgptr, false);
	LevelScene.Add("door", std::move(go));

	go = std::make_unique<engine::GameObject>(glm::vec3(16.f * 10, 16.f * 5, 0.f));
	go->AddComponent<engine::TextureComponent>(std::make_unique<engine::TextureComponent>(go.get(), "destructible.png"));
	go->AddComponent<ColliderComponent>(std::make_unique<ColliderComponent>(go.get(),
		go->GetComponent<engine::TextureComponent>()->GetTextureSize(), true, CollisionType::Destructable));
	go->SetParent(bgptr, false);
	LevelScene.Add("destructible2", std::move(go));

	auto& generalScene = engine::sceneManager::CreateScene("");
	auto gameStateHolder = std::make_unique<engine::GameObject>();
	gameStateHolder->AddComponent(std::make_unique<StateComponent<GameStateInterface>>(gameStateHolder.get(), new StartMenuState()));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_M, engine::KeyState::Pressed, std::make_unique<engine::MuteVolumeCommand>());
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_COMMA, engine::KeyState::Pressed, std::make_unique<engine::AlterVolumeCommand>(-5));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_PERIOD, engine::KeyState::Pressed, std::make_unique<engine::AlterVolumeCommand>(5));
	generalScene.Add("game state", std::move(gameStateHolder));
}

int main(int, char* []) {
	engine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}