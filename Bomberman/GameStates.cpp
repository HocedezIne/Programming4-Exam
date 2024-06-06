#include "GameStates.h"

#include <algorithm>

#include "InputCommandLinker.h"
#include "BombCommand.h"
#include "RespawnBombermanCommand.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "CollisionSystem.h"
#include "Renderer.h"
#include "TimeService.h"
#include "DataComponent.h"
#include "EnemyController.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "GameObject.h"
#include "HighscoreData.h"
#include "UILinkingComponent.h"
#include "TimerComponent.h"

int GameStateInterface::m_CurrentLevel = 1;
const int GameStateInterface::m_TotalLevels = 3;

#pragma region StartMenuState
GameStateInterface* StartMenuState::HandleInput()
{
	auto& input = engine::InputCommandLinker::GetInstance();

	if (input.IsKeyDown(SDL_SCANCODE_RETURN))
	{
		if(m_Cursor->GetLocalPosition() == m_Start) return new LevelLoadingState();
		else if(m_Cursor->GetLocalPosition() == m_Controls) return new ControlsMenuState();
	}

	return nullptr;
}

void StartMenuState::Update()
{
		
}

void StartMenuState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScene =  engine::sceneManager::sceneMap["Start menu"].get();
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/StartMenu.mp3", true);

	engine::sceneManager::currentScene->GetObject("score")->GetComponent<engine::TextComponent>()->SetText(highscoreData::GetHighscoreString());

	m_Cursor = engine::sceneManager::currentScene->GetObject("cursor");
	assert(m_Cursor);
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Pressed, std::make_unique<engine::SnapToInputCommand>(m_Cursor, m_Start));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Pressed, std::make_unique<engine::SnapToInputCommand>(m_Cursor, m_Controls));
}

void StartMenuState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}
#pragma endregion StartMenuState

#pragma region LevelState
GameStateInterface* LevelState::HandleInput()
{
	if (engine::InputCommandLinker::GetInstance().IsKeyDown(SDL_SCANCODE_F1) || m_Won)
		return new LevelWonState();
	if (m_Lost)
		return new LevelLostState();

	return nullptr;
}

void LevelState::Update()
{
		
}

void LevelState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(173, 173, 173));

	engine::sceneManager::currentScene = engine::sceneManager::sceneMap["Demo level"].get();
	engine::sceneManager::currentScene->EnableUpdates(true);
	engine::sceneManager::currentScene->GetObject("timer")->GetComponent<engine::TimerComponent>()->Reset();

	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/LevelBackground.mp3", true);
	collisionSystem::collisionHandler.AddObserver(this);

	// add player commands
	auto obj = engine::sceneManager::currentScene->GetObject("player1");
	if (obj != nullptr)
	{
		// reset player
		obj->SetLocalPosition(glm::vec3{ 16, 16, 0.f });

		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 0.f,-1.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ -1.f,0.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 0.f,1.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held, std::make_unique<engine::MoveInputCommand>(obj, glm::vec3{ 1.f,0.f,0.f }, 50.f));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed, std::make_unique<PlaceBombCommand>(obj, 16));
		engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_R, engine::KeyState::Pressed, std::make_unique<RespawnBombermanCommand>(obj));
	}
}

void LevelState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();

	engine::sceneManager::currentScene->EnableUpdates(false);

	// remove player commands
	auto obj = engine::sceneManager::currentScene->GetObject("player1");
	if (obj != nullptr)
	{
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_W, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_S, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Held);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_C, engine::KeyState::Pressed);
		engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_R, engine::KeyState::Pressed);
	}
}

void LevelState::OnNotify(engine::Event event, void* caller, const std::any& /*args*/)
{
	if (event == engine::Event::PlayerDied)
	{
		m_Lost = true;
	}
	if (event == engine::Event::PlayerOnExit)
	{
		auto door = static_cast<engine::GameObject*>(caller);
		auto isDoorBlocked = std::any_cast<bool>(door->GetComponent<DataComponent>()->GetData("BLOCKED"));
		if (enemyController::EnemyController::GetInstance().GetCount() == 0 && !isDoorBlocked)
			m_Won = true;
	}
}
#pragma endregion LevelState

#pragma region LevelLoadingState
GameStateInterface* LevelLoadingState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
		return new LevelState();

	return nullptr;
}

void LevelLoadingState::Update()
{
	m_TimeToStateSwitch -= engine::TimeService::GetInstance().GetDeltaTime();
}

void LevelLoadingState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));

	engine::sceneManager::currentScene = engine::sceneManager::sceneMap["Level loading"].get();
	auto text = engine::sceneManager::sceneMap["Level loading"].get()->GetObject("level text");
	text->GetComponent<engine::TextComponent>()->SetText("STAGE " + std::to_string(m_CurrentLevel));

	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/LevelStart.mp3", true);
}

void LevelLoadingState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}
#pragma endregion LevelLoadingState

#pragma region LevelLostState
GameStateInterface* LevelLostState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
		if (std::any_cast<int>(engine::sceneManager::currentScene->GetObject("player1")->GetComponent<DataComponent>()->GetData("LEFT")) > 0)
			return new LevelLoadingState();
		else
			return new GameOverState();

	return nullptr;
}

void LevelLostState::Update()
{
	m_TimeToStateSwitch -= engine::TimeService::GetInstance().GetDeltaTime();
	if((m_TimeToStateSwitch-3.f) < 1e-5f) engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/LevelLost.mp3", true);
}

void LevelLostState::OnEnter()
{
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/BombermanDeath.wav", false);
}

void LevelLostState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}
#pragma endregion LevelLostState

#pragma region LevelWonState
GameStateInterface* LevelWonState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
	{
		if (m_CurrentLevel == (m_TotalLevels+1))
			return new GameWonState();
		else
			return new LevelLoadingState();
	}
		

	return nullptr;
}

void LevelWonState::Update()
{
	m_TimeToStateSwitch -= engine::TimeService::GetInstance().GetDeltaTime();
}

void LevelWonState::OnEnter()
{
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/LevelFinished.mp3", true);
	++m_CurrentLevel;
}

void LevelWonState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}
#pragma endregion LevelWonState

#pragma region GameOverState
GameStateInterface* GameOverState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
		return new HighScoreState();

	return nullptr;
}

void GameOverState::Update()
{
	m_TimeToStateSwitch -= engine::TimeService::GetInstance().GetDeltaTime();
}

void GameOverState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScene = engine::sceneManager::sceneMap["Game over"].get();
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/GameOver.mp3", true);
}

void GameOverState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}
#pragma endregion GameOverState

#pragma region GameWonState
GameStateInterface* GameWonState::HandleInput()
{
	if (m_TimeToStateSwitch <= 0.f)
		return new HighScoreState();

	return nullptr;
}

void GameWonState::Update()
{
	m_TimeToStateSwitch -= engine::TimeService::GetInstance().GetDeltaTime();
}

void GameWonState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScene = engine::sceneManager::sceneMap["Game won"].get();
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/GameWon.mp3", true);
}

void GameWonState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}
#pragma endregion GameWonState

#pragma region ControlsMenuState
GameStateInterface* ControlsMenuState::HandleInput()
{
	if (engine::InputCommandLinker::GetInstance().IsKeyDown(SDL_SCANCODE_RETURN))
	{
		return new StartMenuState();
	}

	return nullptr;
}

void ControlsMenuState::Update()
{

}

void ControlsMenuState::OnEnter()
{
	engine::sceneManager::currentScene = engine::sceneManager::sceneMap["Controls menu"].get();
}

void ControlsMenuState::OnExit()
{
		
}
#pragma endregion ControlsMenuState

#pragma region HighScoreState
GameStateInterface* HighScoreState::HandleInput()
{
	auto& input = engine::InputCommandLinker::GetInstance();

	if (input.IsKeyDown(SDL_SCANCODE_RETURN))
	{
		return new StartMenuState();
	}
	if (input.IsKeyDown(SDL_SCANCODE_D) && m_Cursors!=nullptr)
	{
		auto pos = m_Cursors->GetLocalPosition();
		pos.x -= 516.f;
		pos.x += 17.f;
		pos.x = float(int(pos.x) % 51) + 516.f;
		m_Cursors->SetLocalPosition(pos);
	}
	if (input.IsKeyDown(SDL_SCANCODE_A) && m_Cursors != nullptr)
	{
		auto pos = m_Cursors->GetLocalPosition();
		pos.x -= 516.f;
		pos.x -= 17.f;
		pos.x = (pos.x >= 0) ? pos.x : 34;
		pos.x += 516.f;
		m_Cursors->SetLocalPosition(pos);
	}
	if (input.IsKeyDown(SDL_SCANCODE_W) && m_Cursors != nullptr)
	{
		int Xpos = int(m_Cursors->GetLocalPosition().x -516.f);

		auto datacomp = m_Name->GetComponent<DataComponent>();
		auto nameString = std::any_cast<std::string>(datacomp->GetData("currName"));
		nameString[Xpos / 16] = std::max(nameString[Xpos / 16] - 'A' + 1, 0) % 26 + 'A';
		datacomp->UpdateData("currName", nameString);
		m_Name->GetComponent<engine::TextComponent>()->SetText(nameString);
	}
	if (input.IsKeyDown(SDL_SCANCODE_S) && m_Cursors != nullptr)
	{
		int Xpos = int(m_Cursors->GetLocalPosition().x - 516.f);

		auto datacomp = m_Name->GetComponent<DataComponent>();
		auto nameString = std::any_cast<std::string>(datacomp->GetData("currName"));
		nameString[Xpos / 16] = nameString[Xpos / 16] -'A' - 1;
		nameString[Xpos / 16] = (nameString[Xpos / 16] >= 0) ? nameString[Xpos / 16] : 25;
		nameString[Xpos / 16] += 'A';
		datacomp->UpdateData("currName", nameString);
		m_Name->GetComponent<engine::TextComponent>()->SetText(nameString);
	}

	return nullptr;
}

void HighScoreState::Update()
{

}

void HighScoreState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScene = engine::sceneManager::sceneMap["Highscore menu"].get();

	auto currentScore = std::any_cast<int>(engine::sceneManager::sceneMap["Demo level"].get()->GetObject("player1")->GetComponent<DataComponent>()->GetData("SCORE"));
	engine::sceneManager::sceneMap["Highscore menu"].get()->GetObject("player1score")->GetComponent<engine::TextComponent>()->SetText(std::to_string(currentScore)); 

	m_ScoreData = highscoreData::GetLeaderboardData();
	m_ScoreData.emplace_back(std::make_pair(static_cast<uint16_t>(currentScore/100), "???"));
	std::sort(m_ScoreData.begin(), m_ScoreData.end(), [](const std::pair<uint16_t, std::string>& a, const std::pair<uint16_t, std::string>& b)
		{
			return a.first > b.first;
		});

	for (int rank{ 1 }; rank < std::min(6,std::ssize(m_ScoreData)+1); ++rank)
	{
		float yPos = 138.f + rank * 56.f;
		
		// create rank number
		auto ranknumber = std::make_unique<engine::GameObject>(glm::vec3{ 148.f, yPos,0.f });
		ranknumber->AddComponent(std::make_unique<engine::TextComponent>(ranknumber.get(), std::to_string(rank)));
		engine::sceneManager::currentScene->Add("rank " + std::to_string(rank), std::move(ranknumber));
		
		// create score
		auto score = std::make_unique<engine::GameObject>(glm::vec3{ 300.f, yPos,0.f });
		score->AddComponent(std::make_unique<engine::TextComponent>(score.get(), std::to_string(m_ScoreData[rank-1].first) + "00"));
		engine::sceneManager::currentScene->Add("score " + std::to_string(rank), std::move(score));		

		// if name empty -> add controls and visulizers to change letters
		if (m_ScoreData[rank - 1].second == "???")
		{
			auto upcursor = std::make_unique<engine::GameObject>(glm::vec3{ 516.f, yPos-24.f, 0.f });
			upcursor->AddComponent(std::make_unique<engine::TextureComponent>(upcursor.get(), "arrow up.png"));

			auto downcursor = std::make_unique<engine::GameObject>(glm::vec3{ 0.f, 48.f, 0.f });
			downcursor->AddComponent(std::make_unique<engine::TextureComponent>(downcursor.get(), "arrow down.png"));
			downcursor->SetParent(upcursor.get(), false);
			engine::sceneManager::currentScene->Add("downcursor", std::move(downcursor));

			m_Cursors = upcursor.get();
			engine::sceneManager::currentScene->Add("upcursor", std::move(upcursor));

			auto leftcursor = std::make_unique<engine::GameObject>(glm::vec3{ 492.f, yPos, 0.f });
			leftcursor->AddComponent(std::make_unique<engine::TextureComponent>(leftcursor.get(), "arrow left.png"));
			engine::sceneManager::currentScene->Add("leftcursor", std::move(leftcursor));
			auto rightcursor = std::make_unique<engine::GameObject>(glm::vec3{ 572.f, yPos, 0.f });
			rightcursor->AddComponent(std::make_unique<engine::TextureComponent>(rightcursor.get(), "arrow right.png"));
			engine::sceneManager::currentScene->Add("rightcursor", std::move(rightcursor));

			auto name = std::make_unique<engine::GameObject>(glm::vec3{ 516.f, yPos,0.f });
			auto sc = std::make_unique<DataComponent>(name.get());
			sc->AddDataMapping("currName", m_ScoreData[rank - 1].second);
			name->AddComponent(std::make_unique<engine::TextComponent>(name.get(), m_ScoreData[rank - 1].second));
			name->AddComponent(std::move(sc));

			m_Name = name.get();
			engine::sceneManager::currentScene->Add("currName", std::move(name));

			m_CurrScoreIdx = static_cast<uint8_t>(rank - 1);
		}
		else 
		{
			auto name = std::make_unique<engine::GameObject>(glm::vec3{ 516.f, yPos,0.f });
			name->AddComponent(std::make_unique<engine::TextComponent>(name.get(), m_ScoreData[rank - 1].second));
			engine::sceneManager::currentScene->Add("name " + std::to_string(rank), std::move(name));
		}
	}
}

void HighScoreState::OnExit()
{
	engine::sceneManager::currentScene->Remove("upcursor");
	engine::sceneManager::currentScene->Remove("downcursor");
	engine::sceneManager::currentScene->Remove("leftcursor");
	engine::sceneManager::currentScene->Remove("rightcursor");

	for (int rank{ 1 }; rank < std::min(6, std::ssize(m_ScoreData) + 1); ++rank)
	{
		engine::sceneManager::currentScene->Remove("rank " + std::to_string(rank));
		engine::sceneManager::currentScene->Remove("score " + std::to_string(rank));

		if (m_ScoreData[rank - 1].second == "???")
		{
			m_ScoreData[rank - 1].second = std::any_cast<std::string>(engine::sceneManager::currentScene->GetObject("currName")->GetComponent< DataComponent>()->GetData("currName"));
			if (m_ScoreData[rank - 1].second == "???") m_ScoreData[rank + 1].second = "UNK";
			engine::sceneManager::currentScene->Remove("currName");
		}
		else engine::sceneManager::currentScene->Remove("name " + std::to_string(rank));
	}

	engine::sceneManager::currentScene->HandleDeletion();

	if (m_ScoreData.size() > 5)
		m_ScoreData.erase(m_ScoreData.begin() + 5, m_ScoreData.end());

	highscoreData::SaveLeaderboardData(m_ScoreData);

	m_CurrentLevel = 1;
}
#pragma endregion HighScoreState