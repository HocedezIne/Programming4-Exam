#pragma once

#include "EnemyStateInterface.h"

class BalloomWalking final : public EnemyStateWalking
{
public:
	BalloomWalking(engine::GameObject* enemy) :
		EnemyStateWalking(25.f, enemy)
	{}

	virtual ~BalloomWalking() = default;

	virtual EnemyStateInterface* HandleInput() override
	{
		return nullptr;
	}
};