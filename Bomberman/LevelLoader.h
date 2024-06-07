#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace levelLoader
{
	static void LoadLevel(std::string& filePath);

	static std::vector<glm::vec2> m_UsedPositions;
}