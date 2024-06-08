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
	m_TimeToStateSwitch -= engine::TimeUtil::GetDeltaTime();
}

void LevelLoadingState::OnEnter()
{
	engine::Renderer::GetInstance().SetBackgroundColor(SDL_Color(0, 0, 0));

	engine::sceneManager::currentScenes.push_back(engine::sceneManager::sceneMap["Level loading"].get());
	auto text = engine::sceneManager::sceneMap["Level loading"].get()->GetObject("level text");
	if (m_GameMode != GameMode::Vs) text->GetComponent<engine::TextComponent>()->SetText("STAGE " + std::to_string(m_CurrentLevel));
	else text->GetComponent<engine::TextComponent>()->SetText("GET READY");

	engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/Sounds/LevelStart.mp3", true);
}

void LevelLoadingState::OnExit()
{
	engine::ServiceLocator::GetSoundSystem().StopAllSound();
	engine::sceneManager::currentScenes.clear();
}