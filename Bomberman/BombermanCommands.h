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

class RespawnBombermanCommand : public engine::GameObjectInputCommand
{
public:
	RespawnBombermanCommand(engine::GameObject* go) : engine::GameObjectInputCommand(go) {}

	virtual ~RespawnBombermanCommand() = default;

	RespawnBombermanCommand(const RespawnBombermanCommand& other) = delete;
	RespawnBombermanCommand& operator=(const RespawnBombermanCommand& other) = delete;
	RespawnBombermanCommand(RespawnBombermanCommand&& other) = delete;
	RespawnBombermanCommand& operator=(RespawnBombermanCommand&& other) = delete;

	virtual void Execute() override {
		GetGameObject()->SetLocalPosition(glm::vec3{ 16, 116, 0.f });
	};
};