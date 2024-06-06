#pragma once

#include "EnemyStateInterface.h"

class DollWalking final : public EnemyStateWalking
{
public:
	DollWalking(engine::GameObject* enemy) :
		EnemyStateWalking(50.f, enemy)
	{}

	virtual ~DollWalking() = default;

	virtual EnemyStateInterface* HandleInput() override
	{
		return nullptr;
	}
};