#include "GameStates.h"

#include "InputCommandLinker.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Scene.h"

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
	engine::sceneManager::currentScenes.push_back(engine::sceneManager::sceneMap["Controls menu"].get());
}

void ControlsMenuState::OnExit()
{
	engine::sceneManager::currentScenes.clear();
}