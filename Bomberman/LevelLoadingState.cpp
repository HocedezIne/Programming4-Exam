#include "GameStates.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

#include "TextComponent.h"

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

	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/LevelStart.mp3", true);
}

void LevelLoadingState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
}