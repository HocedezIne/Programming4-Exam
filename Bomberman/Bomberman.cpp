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
	logo->AddComponent(std::make_unique<engine::TextureComponent>(logo.get(), "Images/StartMenu.png"));
	startScene.Add("logo", std::move(logo));

	auto cursor = std::make_unique<engine::GameObject>(glm::vec3{ 144.f, 304.f, 0.f });
	cursor->AddComponent(std::make_unique<engine::TextureComponent>(cursor.get(), "Images/arrow right.png"));
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
	highScore->AddComponent(std::make_unique<engine::TextComponent>(highScore.get(), "0"));
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

#pragma region GameModeSelect
	auto& gmScene = engine::sceneManager::CreateScene("Game mode selection");

	text = std::make_unique<engine::GameObject>(glm::vec3{ 175.f,50.f,0.f });
	text->AddComponent(std::make_unique<engine::TextComponent>(text.get(), "PICK A GAME MODE"));
	gmScene.Add("text", std::move(text));

	auto cursorRight = std::make_unique<engine::GameObject>(glm::vec3{ 176.f, 175.f, 0.f });
	cursorRight->AddComponent(std::make_unique<engine::TextureComponent>(cursorRight.get(), "Images/arrow right.png"));
	auto cursorLeft = std::make_unique<engine::GameObject>(glm::vec3{ 269.f, 0.f, 0.f });
	cursorLeft->AddComponent(std::make_unique<engine::TextureComponent>(cursorLeft.get(), "Images/arrow left.png"));
	cursorLeft->SetParent(cursorRight.get(), false);
	gmScene.Add("cursorLeft", std::move(cursorLeft));
	gmScene.Add("cursors", std::move(cursorRight));

	text = std::make_unique<engine::GameObject>(glm::vec3{ 200.f, 175.f, 0.f });
	text->AddComponent(std::make_unique<engine::TextComponent>(text.get(), "SINGLE PLAYER"));
	gmScene.Add("single", std::move(text));

	text = std::make_unique<engine::GameObject>(glm::vec3{ 275.f,239.f,0.f });
	text->AddComponent(std::make_unique<engine::TextComponent>(text.get(), "CO-OP"));
	gmScene.Add("coop", std::move(text));

	text = std::make_unique<engine::GameObject>(glm::vec3{ 267.f,303.f,0.f });
	text->AddComponent(std::make_unique<engine::TextComponent>(text.get(), "VERSUS"));
	gmScene.Add("versus", std::move(text));

#pragma endregion GameModeSelect

#pragma region Controls
	auto& controlsMenu = engine::sceneManager::CreateScene("Controls menu");
	auto fontBig = engine::ResourceManager::GetInstance().LoadFont("Fonts/nes-arcade-font-monospace.otf", 24);
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

	auto player1score = std::make_unique<engine::GameObject>(glm::vec3{ 450.f, 82.f,0.f });
	player1score->AddComponent(std::make_unique<engine::TextComponent>(player1score.get(), ""));
	highscoreScene.Add("player1score", std::move(player1score));

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

	auto& generalScene = engine::sceneManager::CreateScene("");
	auto gameStateHolder = std::make_unique<engine::GameObject>();
	gameStateHolder->AddComponent(std::make_unique<StateComponent<GameStateInterface>>(gameStateHolder.get(), new StartMenuState()));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_M, engine::KeyState::Pressed, std::make_unique<engine::MuteVolumeCommand>());
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_COMMA, engine::KeyState::Pressed, std::make_unique<engine::AlterVolumeCommand>(-5));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_PERIOD, engine::KeyState::Pressed, std::make_unique<engine::AlterVolumeCommand>(5));
	generalScene.Add("game state", std::move(gameStateHolder));

	engine::InputCommandLinker::GetInstance().AddKeyboard();
	engine::InputCommandLinker::GetInstance().AddController();
}

int main(int, char* []) {
	engine::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}