#pragma once

#include "InputCommands.h"

#include "BombControllerComponent.h"

class PlaceBombCommand : public engine::GameObjectInputCommand
{
public:
	PlaceBombCommand(engine::GameObject* go, int gridSize) : engine::GameObjectInputCommand(go), m_GridSize(gridSize) {}

	virtual ~PlaceBombCommand() = default;

	PlaceBombCommand(const PlaceBombCommand& other) = delete;
	PlaceBombCommand& operator=(const PlaceBombCommand& other) = delete;
	PlaceBombCommand(PlaceBombCommand&& other) = delete;
	PlaceBombCommand& operator=(PlaceBombCommand&& other) = delete;

	virtual void Execute() override { 
		auto bombPos = GetGameObject()->GetWorldPosition();
		bombPos.x = std::roundf(bombPos.x / m_GridSize) * m_GridSize;
		bombPos.y = std::roundf(bombPos.y / m_GridSize) * m_GridSize;

		GetGameObject()->GetComponent<BombControllerComponent>()->AddBomb(bombPos);
	};

private:
	const int m_GridSize;
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