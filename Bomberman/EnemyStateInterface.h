#pragma once

#include "InputCommands.h"
#include "Observer.h"

class EnemyStateInterface : public engine::Observer
{
public:
	EnemyStateInterface() = default;
	virtual ~EnemyStateInterface() = default;

	virtual EnemyStateInterface* HandleInput() = 0;
	virtual void Update() = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;

	virtual void OnNotify(engine::Event event, void* caller, const std::any& args) = 0;
};

class EnemyStateWalking : public EnemyStateInterface
{
public:
	EnemyStateWalking(float speed, engine::GameObject* enemy);

	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void OnNotify(engine::Event event, void* caller, const std::any& args) override;

private:
	void ChangeDirection();

	glm::vec3 m_Direction{};
	const float m_Speed;
	engine::GameObject* m_EnemyObject;
	std::unique_ptr<engine::MoveInputCommand> m_MoveCommand;
};