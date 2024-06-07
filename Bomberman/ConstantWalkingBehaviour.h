#pragma once

#include "EnemyStateInterface.h"

class ConstantWalking final : public EnemyStateWalking
{
public:
	ConstantWalking(float speed, engine::GameObject* enemy,
		const std::vector<engine::GameObject*> players, float chasingDistance = 0) :
		EnemyStateWalking(speed, enemy, players, chasingDistance)
	{}

	virtual ~ConstantWalking() = default;

	virtual EnemyStateInterface* HandleInput() override
	{
		return nullptr;
	}
};