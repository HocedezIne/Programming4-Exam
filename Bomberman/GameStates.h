#pragma once
#include "GameObject.h"
#include "Observer.h"

class GameStateInterface {
public:
	GameStateInterface() = default;
	virtual ~GameStateInterface() = default;

	virtual GameStateInterface* HandleInput() = 0;
	virtual void Update() = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

protected:
	static int m_CurrentLevel;
};

class StartMenuState final : public GameStateInterface
{
public:
	virtual GameStateInterface* HandleInput() override;
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

private:
	engine::GameObject* m_Cursor;
	const glm::vec3 m_Start{ 144, 304, 0 };
	const glm::vec3 m_Controls{ 320, 304, 0 };
};

class LevelLoadingState final : public GameStateInterface
{
public:
	virtual GameStateInterface* HandleInput() override;
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

private:
	float m_TimeToStateSwitch{ 3.f };
};

class LevelState final : public GameStateInterface, public engine::Observer
{
public:
	virtual GameStateInterface* HandleInput() override;
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void OnNotify(engine::Event event, void* caller, const std::any& args) override;

private:
	bool m_Lost{ false };
	bool m_Won{ false };
};

class LevelLostState final : public GameStateInterface
{
public:
	virtual GameStateInterface* HandleInput() override;
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

private:
	float m_TimeToStateSwitch{ 4.f };
};

class LevelWonState final : public GameStateInterface
{
public:
	virtual GameStateInterface* HandleInput() override;
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

private:
	float m_TimeToStateSwitch{ 3.f };
};

class ControlsMenuState final : public GameStateInterface
{
public:
	virtual GameStateInterface* HandleInput() override;
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};

class GameOverState final : public GameStateInterface
{
public:
	virtual GameStateInterface* HandleInput() override;
	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};