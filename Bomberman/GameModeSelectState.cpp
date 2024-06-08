#include "GameStates.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

GameStateInterface* GameModeSelectState::HandleInput()
{
	auto& input = engine::InputCommandLinker::GetInstance();

	if (input.IsKeyDown(SDL_SCANCODE_RETURN) || input.IsButtonDown(engine::Controller::Button::A))
	{
		return new LevelLoadingState();
	}

	if ((input.IsKeyDown(SDL_SCANCODE_S) || input.IsButtonDown(engine::Controller::Button::DPadDown)) && m_Cursors != nullptr)
	{
		auto pos = m_Cursors->GetLocalPosition();
		pos.y -= 175.f;
		pos.y += 64.f;
		pos.y = float(int(pos.y) % 192) + 175.f;
		m_Cursors->SetLocalPosition(pos);
	}
	if ((input.IsKeyDown(SDL_SCANCODE_W) || input.IsButtonDown(engine::Controller::Button::DPadUp)) && m_Cursors != nullptr)
	{
		auto pos = m_Cursors->GetLocalPosition();
		pos.y -= 175.f;
		pos.y -= 64.f;
		pos.y = (pos.y >= 0) ? pos.y : 128;
		pos.y += 175.f;
		m_Cursors->SetLocalPosition(pos);
	}

	return nullptr;
}

void GameModeSelectState::Update() {}

void GameModeSelectState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScenes.push_back(engine::sceneManager::sceneMap["Game mode selection"].get());

	m_Cursors = engine::sceneManager::currentScenes[0]->GetObject("cursors");
}

void GameModeSelectState::OnExit()
{
	// set game mode
	auto pos = m_Cursors->GetLocalPosition();
	pos.y -= 175.f;
	m_GameMode = static_cast<GameMode>(static_cast<int>(pos.y / 64));

	// create players per game mode
	switch (m_GameMode)
	{
	case GameMode::Single:

		break;
	case GameMode::Coop:

		break;
	case GameMode::Vs:

		break;
	default:
		break;
	}

	engine::sceneManager::currentScenes.clear();
}