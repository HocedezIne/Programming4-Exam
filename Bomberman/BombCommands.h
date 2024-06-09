#pragma once

#include "InputCommands.h"

#include "BombControllerComponent.h"
#include "LevelLoader.h"

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
		auto bombPos = GetGameObject()->GetWorldPosition();
		bombPos.x = std::roundf(bombPos.x / levelLoader::m_GridSize) * levelLoader::m_GridSize - levelLoader::m_GridSize/2;
		bombPos.y = std::roundf(bombPos.y / levelLoader::m_GridSize) * levelLoader::m_GridSize;

		GetGameObject()->GetComponent<BombControllerComponent>()->AddBomb(bombPos);
	};
};

class DetonateCommand : public engine::GameObjectInputCommand
{
public:
	DetonateCommand(engine::GameObject* go) : engine::GameObjectInputCommand(go) {};

	virtual ~DetonateCommand() = default;

	DetonateCommand(const DetonateCommand& other) = delete;
	DetonateCommand& operator=(const DetonateCommand& other) = delete;
	DetonateCommand(DetonateCommand&& other) = delete;
	DetonateCommand& operator=(DetonateCommand&& other) = delete;

	virtual void Execute() override {
		GetGameObject()->GetComponent<BombControllerComponent>()->ExplodeAllBombs();
	};
};