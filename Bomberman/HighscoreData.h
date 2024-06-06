#pragma once

#include <string>
#include <vector>

namespace highscoreData
{
	std::string GetHighscoreString();
	std::vector<std::pair<uint16_t, std::string>> GetLeaderboardData();
	void SaveLeaderboardData(std::vector<std::pair<uint16_t, std::string>> data);
}