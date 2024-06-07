#pragma once

#include "EnemyStateInterface.h"

class WalkTillChasing final : public EnemyStateWalking
{
public:
	WalkTillChasing(float speed, engine::GameObject* enemy, 
		const std::vector<engine::GameObject*> players,float chasingDistance) :
		EnemyStateWalking(speed, enemy, players, chasingDistance)
	{}

	virtual ~WalkTillChasing() = default;

	virtual EnemyStateInterface* HandleInput() override;
};

class Chasing final : public EnemyStateChasing
{
public:
	Chasing(float speed, engine::GameObject* enemy,
		const std::vector<engine::GameObject*> players, float chasingDistance, int index) :
		EnemyStateChasing(speed, enemy, players, chasingDistance, index)
	{}

	virtual ~Chasing() = default;

	virtual EnemyStateInterface* HandleInput();
};