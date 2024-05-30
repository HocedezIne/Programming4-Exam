#pragma once
#include "Observer.h"

class GameStateInterface {
public:
	GameStateInterface() = default;
	virtual ~GameStateInterface() = default;

	virtual GameStateInterface* HandleInput() = 0;
	virtual void Update() = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
};

class StartMenuState : public GameStateInterface
{
public:
	virtual GameStateInterface* HandleInput() override;
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};

class LevelLoadingState : public GameStateInterface
{
public:
	virtual GameStateInterface* HandleInput() override;
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

private:
	float m_TimeToStateSwitch{ 3.f };
};

class LevelState : public GameStateInterface, public engine::Observer
{
public:
	virtual GameStateInterface* HandleInput() override;
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void OnNotify(engine::Event event, void* caller, const std::any& args) override;
};

class ControlsMenuState : public GameStateInterface
{
public:
	virtual GameStateInterface* HandleInput() override;
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};