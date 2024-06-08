#pragma once

#include <string>
#include <vector>

#include <json.hpp>
#include <glm/glm.hpp>

#include "Scene.h"
#include "GameObject.h"
#include "GameStates.h"

using json = nlohmann::json;

namespace levelLoader
{
	void LoadLevel(const std::string filePath);
	void LoadStatics(GameMode gameMode);
	static void LoadRequiredPlayers(GameMode gameMode);

	static void CreateSoftBlocks(engine::GameObject* parent, engine::Scene& scene, int count);
	static void CreateDoor(engine::GameObject* parent, engine::Scene& scene);
	static void CreatePowerUp(engine::GameObject* parent, engine::Scene& scene, std::string type);
	static void CreateEnemies(engine::GameObject* parent, engine::Scene& scene, const json& enemiesCollection);

	static glm::vec2 GetFreePosition();

	extern std::vector<glm::vec2> m_UsedPositions;
}