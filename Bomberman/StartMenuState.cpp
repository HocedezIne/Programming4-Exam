#include "GameStates.h"

#include "HighscoreData.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

#include "TextComponent.h"

GameStateInterface* StartMenuState::HandleInput()
{
	auto& input = engine::InputCommandLinker::GetInstance();

	if (input.IsKeyDown(SDL_SCANCODE_RETURN) || input.IsButtonDown(engine::Controller::Button::A))
	{
		if (m_Cursor->GetLocalPosition() == m_Start) return new GameModeSelectState();
		else if (m_Cursor->GetLocalPosition() == m_Controls) return new ControlsMenuState();
	}

	return nullptr;
}

void StartMenuState::Update()
{

}

void StartMenuState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));
	engine::sceneManager::currentScenes.push_back(engine::sceneManager::sceneMap["Start menu"].get());
	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/StartMenu.mp3", true);

	engine::sceneManager::currentScenes[0]->GetObject("score")->GetComponent<engine::TextComponent>()->SetText(highscoreData::GetHighscoreString());

	m_Cursor = engine::sceneManager::currentScenes[0]->GetObject("cursor");
	assert(m_Cursor);
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Pressed, std::make_unique<engine::SnapToInputCommand>(m_Cursor, m_Start));
	engine::InputCommandLinker::GetInstance().AddKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Pressed, std::make_unique<engine::SnapToInputCommand>(m_Cursor, m_Controls));

	engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadLeft, engine::KeyState::Pressed, std::make_unique<engine::SnapToInputCommand>(m_Cursor, m_Start));
	engine::InputCommandLinker::GetInstance().AddControllerCommand(engine::Controller::Button::DPadRight, engine::KeyState::Pressed, std::make_unique<engine::SnapToInputCommand>(m_Cursor, m_Controls));
}

void StartMenuState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();

	engine::sceneManager::currentScenes.clear();

	engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_A, engine::KeyState::Pressed);
	engine::InputCommandLinker::GetInstance().RemoveKeyboardCommand(SDL_SCANCODE_D, engine::KeyState::Pressed);
	engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadLeft, engine::KeyState::Pressed);
	engine::InputCommandLinker::GetInstance().RemoveControllerCommand(engine::Controller::Button::DPadRight, engine::KeyState::Pressed);
}
