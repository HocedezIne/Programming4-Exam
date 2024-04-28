#pragma once

#include "InputCommands.h"
#include "ServiceLocator.h"

class PlaceBombCommand : public engine::GameObjectInputCommand
{
public:
	PlaceBombCommand(engine::GameObject* go) : engine::GameObjectInputCommand(go) {}

	virtual ~PlaceBombCommand() = default;

	PlaceBombCommand(const PlaceBombCommand& other) = delete;
	PlaceBombCommand& operator=(const PlaceBombCommand& other) = delete;
	PlaceBombCommand(PlaceBombCommand&& other) = delete;
	PlaceBombCommand& operator=(PlaceBombCommand&& other) = delete;

	virtual void Execute() override { 
		engine::ServiceLocator::GetSoundSystem().PlaySound("../Data/BombermanExplosion.wav", false); 
	};
};