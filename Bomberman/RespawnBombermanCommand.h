#pragma once

#include "InputCommands.h"

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

		GetGameObject()->SetLocalPosition(glm::vec3{ 16, 16, 0.f });
	};
};