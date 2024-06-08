#include "GameStates.h"

#include <algorithm>

#include "HighscoreData.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

#include "DataComponent.h"
#include "TextComponent.h"
#include "TextureComponent.h"

GameStateInterface* HighScoreState::HandleInput()
{
	auto& input = engine::InputCommandLinker::GetInstance();

	if (input.IsKeyDown(SDL_SCANCODE_RETURN) || input.IsButtonDown(engine::Controller::Button::A))
	{
		return new StartMenuState();
	}
	if ((input.IsKeyDown(SDL_SCANCODE_D) || input.IsButtonDown(engine::Controller::Button::DPadRight)) && m_Cursors != nullptr)
	{
		auto pos = m_Cursors->GetLocalPosition();
		pos.x -= 516.f;
		pos.x += 17.f;
		pos.x = float(int(pos.x) % 51) + 516.f;
		m_Cursors->SetLocalPosition(pos);
	}
	if ((input.IsKeyDown(SDL_SCANCODE_A) || input.IsButtonDown(engine::Controller::Button::DPadLeft)) && m_Cursors != nullptr)
	{
		auto pos = m_Cursors->GetLocalPosition();
		pos.x -= 516.f;
		pos.x -= 17.f;
		pos.x = (pos.x >= 0) ? pos.x : 34;
		pos.x += 516.f;
		m_Cursors->SetLocalPosition(pos);
	}
	if ((input.IsKeyDown(SDL_SCANCODE_W) || input.IsButtonDown(engine::Controller::Button::DPadUp)) && m_Cursors != nullptr)
	{
		int Xpos = int(m_Cursors->GetLocalPosition().x - 516.f);

		auto datacomp = m_Name->GetComponent<DataComponent>();
		auto nameString = std::any_cast<std::string>(datacomp->GetData("currName"));
		nameString[Xpos / 16] = std::max(nameString[Xpos / 16] - 'A' + 1, 0) % 26 + 'A';
		datacomp->UpdateData("currName", nameString);
		m_Name->GetComponent<engine::TextComponent>()->SetText(nameString);
	}
	if ((input.IsKeyDown(SDL_SCANCODE_S) || input.IsButtonDown(engine::Controller::Button::DPadDown)) && m_Cursors != nullptr)
	{
		int Xpos = int(m_Cursors->GetLocalPosition().x - 516.f);

		auto datacomp = m_Name->GetComponent<DataComponent>();
		auto nameString = std::any_cast<std::string>(datacomp->GetData("currName"));
		nameString[Xpos / 16] = nameString[Xpos / 16] - 'A' - 1;
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
	if(m_GameMode != GameMode::Single) engine::InputCommandLinker::GetInstance().RemoveController(1);

	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScenes.push_back(engine::sceneManager::sceneMap["Highscore menu"].get());

	int currentHighestPlayerScore{};
	switch (m_GameMode)
	{
	case GameMode::Single:
	{
		currentHighestPlayerScore = std::any_cast<int>(engine::sceneManager::sceneMap["Level Statics"].get()->GetObject("player1")->GetComponent<DataComponent>()->GetData("SCORE"));
		engine::sceneManager::sceneMap["Highscore menu"].get()->GetObject("player1score")->GetComponent<engine::TextComponent>()->SetText(std::to_string(currentHighestPlayerScore));

		break;
	}
	case GameMode::Coop:
	{
		auto p1score = std::any_cast<int>(engine::sceneManager::sceneMap["Level Statics"].get()->GetObject("player1")->GetComponent<DataComponent>()->GetData("SCORE"));
		engine::sceneManager::sceneMap["Highscore menu"].get()->GetObject("player1score")->GetComponent<engine::TextComponent>()->SetText(std::to_string(p1score));

		auto p2score = std::any_cast<int>(engine::sceneManager::sceneMap["Level Statics"].get()->GetObject("player2")->GetComponent<DataComponent>()->GetData("SCORE"));
		auto player2text = std::make_unique<engine::GameObject>(glm::vec3{ 150.f, 106.f,0.f });
		player2text->AddComponent(std::make_unique<engine::TextComponent>(player2text.get(), "PLAYER 2 SCORE"));
		engine::sceneManager::currentScenes[0]->Add("player2text", std::move(player2text));
		auto player2score = std::make_unique<engine::GameObject>(glm::vec3{ 450.f, 106.f,0.f });
		player2score->AddComponent(std::make_unique<engine::TextComponent>(player2score.get(), std::to_string(p2score)));
		engine::sceneManager::currentScenes[0]->Add("player2score", std::move(player2score));

		currentHighestPlayerScore = std::max(p1score, p2score);

		break;
	}
	case GameMode::Vs:
	default:
		engine::sceneManager::sceneMap["Highscore menu"].get()->GetObject("player1text")->GetComponent<engine::TextComponent>()->SetText(" ");
		engine::sceneManager::sceneMap["Highscore menu"].get()->GetObject("player1score")->GetComponent<engine::TextComponent>()->SetText(" ");
		break;
	}

	m_ScoreData = highscoreData::GetLeaderboardData();
	m_ScoreData.emplace_back(std::make_pair(static_cast<uint16_t>(currentHighestPlayerScore / 100), "???"));
	std::sort(m_ScoreData.begin(), m_ScoreData.end(), [](const std::pair<uint16_t, std::string>& a, const std::pair<uint16_t, std::string>& b)
		{
			return a.first > b.first;
		});

	for (int rank{ 1 }; rank < std::min(6, static_cast<int>(std::ssize(m_ScoreData)) + 1); ++rank)
	{
		float yPos = 138.f + rank * 56.f;

		// create rank number
		auto ranknumber = std::make_unique<engine::GameObject>(glm::vec3{ 148.f, yPos,0.f });
		ranknumber->AddComponent(std::make_unique<engine::TextComponent>(ranknumber.get(), std::to_string(rank)));
		engine::sceneManager::currentScenes[0]->Add("rank " + std::to_string(rank), std::move(ranknumber));

		// create score
		auto score = std::make_unique<engine::GameObject>(glm::vec3{ 300.f, yPos,0.f });
		score->AddComponent(std::make_unique<engine::TextComponent>(score.get(), std::to_string(m_ScoreData[rank - 1].first) + "00"));
		engine::sceneManager::currentScenes[0]->Add("score " + std::to_string(rank), std::move(score));

		// if name empty -> add controls and visulizers to change letters
		if (m_ScoreData[rank - 1].second == "???")
		{
			auto upcursor = std::make_unique<engine::GameObject>(glm::vec3{ 516.f, yPos - 24.f, 0.f });
			upcursor->AddComponent(std::make_unique<engine::TextureComponent>(upcursor.get(), "Images/arrow up.png"));

			auto downcursor = std::make_unique<engine::GameObject>(glm::vec3{ 0.f, 48.f, 0.f });
			downcursor->AddComponent(std::make_unique<engine::TextureComponent>(downcursor.get(), "Images/arrow down.png"));
			downcursor->SetParent(upcursor.get(), false);
			engine::sceneManager::currentScenes[0]->Add("downcursor", std::move(downcursor));

			m_Cursors = upcursor.get();
			engine::sceneManager::currentScenes[0]->Add("upcursor", std::move(upcursor));

			auto leftcursor = std::make_unique<engine::GameObject>(glm::vec3{ 492.f, yPos, 0.f });
			leftcursor->AddComponent(std::make_unique<engine::TextureComponent>(leftcursor.get(), "Images/arrow left.png"));
			engine::sceneManager::currentScenes[0]->Add("leftcursor", std::move(leftcursor));
			auto rightcursor = std::make_unique<engine::GameObject>(glm::vec3{ 572.f, yPos, 0.f });
			rightcursor->AddComponent(std::make_unique<engine::TextureComponent>(rightcursor.get(), "Images/arrow right.png"));
			engine::sceneManager::currentScenes[0]->Add("rightcursor", std::move(rightcursor));

			auto name = std::make_unique<engine::GameObject>(glm::vec3{ 516.f, yPos,0.f });
			auto sc = std::make_unique<DataComponent>(name.get());
			sc->AddDataMapping("currName", m_ScoreData[rank - 1].second);
			name->AddComponent(std::make_unique<engine::TextComponent>(name.get(), m_ScoreData[rank - 1].second));
			name->AddComponent(std::move(sc));

			m_Name = name.get();
			engine::sceneManager::currentScenes[0]->Add("currName", std::move(name));

			m_CurrScoreIdx = static_cast<uint8_t>(rank - 1);
		}
		else
		{
			auto name = std::make_unique<engine::GameObject>(glm::vec3{ 516.f, yPos,0.f });
			name->AddComponent(std::make_unique<engine::TextComponent>(name.get(), m_ScoreData[rank - 1].second));
			engine::sceneManager::currentScenes[0]->Add("name " + std::to_string(rank), std::move(name));
		}
	}
}

void HighScoreState::OnExit()
{
	if (m_GameMode == GameMode::Coop)
	{
		engine::sceneManager::currentScenes[0]->Remove("player2text");
		engine::sceneManager::currentScenes[0]->Remove("player2score");
	}
	engine::sceneManager::currentScenes[0]->Remove("upcursor");
	engine::sceneManager::currentScenes[0]->Remove("downcursor");
	engine::sceneManager::currentScenes[0]->Remove("leftcursor");
	engine::sceneManager::currentScenes[0]->Remove("rightcursor");

	for (int rank{ 1 }; rank < std::min(6, static_cast<int>(std::ssize(m_ScoreData)) + 1); ++rank)
	{
		engine::sceneManager::currentScenes[0]->Remove("rank " + std::to_string(rank));
		engine::sceneManager::currentScenes[0]->Remove("score " + std::to_string(rank));

		if (m_ScoreData[rank - 1].second == "???")
		{
			m_ScoreData[rank - 1].second = std::any_cast<std::string>(engine::sceneManager::currentScenes[0]->GetObject("currName")->GetComponent< DataComponent>()->GetData("currName"));
			if (m_ScoreData[rank - 1].second == "???") m_ScoreData[rank + 1].second = "UNK";
			engine::sceneManager::currentScenes[0]->Remove("currName");
		}
		else engine::sceneManager::currentScenes[0]->Remove("name " + std::to_string(rank));
	}

	engine::sceneManager::currentScenes[0]->HandleDeletion();

	if (m_ScoreData.size() > 5)
		m_ScoreData.erase(m_ScoreData.begin() + 5, m_ScoreData.end());

	highscoreData::SaveLeaderboardData(m_ScoreData);

	m_CurrentLevel = 1;
	engine::sceneManager::currentScenes.clear();
}