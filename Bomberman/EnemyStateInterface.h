#pragma once

#include "InputCommands.h"
#include "Observer.h"

class EnemyStateInterface : public engine::Observer
{
public:
	EnemyStateInterface(float speed, engine::GameObject* enemy, 
		const std::vector<engine::GameObject*> players, float chasingDistance);
	virtual ~EnemyStateInterface() = default;

	virtual EnemyStateInterface* HandleInput() = 0;
	virtual void Update() = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit();

	virtual void OnNotify(engine::Event event, void* caller, const std::any& args) override;

protected:
	void ChangeDirection();
	void ChangeDirection(const glm::vec3 direction);

	glm::vec3 m_Direction{};
	const float m_Speed;
	const float m_ChasingDistance;
	engine::GameObject* m_EnemyObject;
	std::unique_ptr<engine::MoveInputCommand> m_MoveCommand;

	const std::vector<engine::GameObject*> m_Players;
};

class EnemyStateWalking : public EnemyStateInterface
{
public:
	EnemyStateWalking(float speed, engine::GameObject* enemy, 
		const std::vector<engine::GameObject*> players, float chasingDistance);

	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
};

class EnemyStateChasing : public EnemyStateInterface
{
public:
	EnemyStateChasing(float speed, engine::GameObject* enemy, 
		const std::vector<engine::GameObject*> players, float chasingDistance, int index);

	virtual void Update() override;
	virtual void OnEnter() override;
	virtual void OnExit() override;
private:
	int m_IndexToChase{};
};