#include "HighscoreData.h"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace highscoreData
{
	std::string GetHighscoreString()
	{
		uint16_t highscore{};

		if (std::ifstream input{ "../Data/Highscores.bin", std::ios::binary }; input.is_open())
		{
			input.read((char*)&highscore, sizeof(highscore));

			input.close();
		}
		else
		{
			std::cout << "Couldn't read highscores.bin\n";
		}

		std::ostringstream result;
		result  << std::to_string(highscore) << "00";
		return result.str();
	}

	std::vector<std::pair<uint16_t, std::string>> GetLeaderboardData()
	{
		std::vector<std::pair<uint16_t, std::string>> result;

		if (std::ifstream input{ "../Data/Highscores.bin", std::ios::binary }; input.is_open())
		{
			uint16_t score{};
			char name[4];

			for (size_t idx{}; idx < 5 && input.read((char*)&score, sizeof(score)); ++idx)
			{
				input.read(name, 3); // name length is always 3 + null terminator
				name[3] = '\0';

				result.emplace_back(std::make_pair(score, std::string(name)));
			}

			input.close();
		}
		else
		{
			std::cout << "Couldn't read highscores.bin\n";
		}

		return result;
	}

	void SaveLeaderboardData(std::vector<std::pair<uint16_t, std::string>> data)
	{
		if (std::ofstream output{ "../Data/Highscores.bin", std::ios::binary }; output.is_open())
		{
			for (auto& item : data)
			{
				output.write((const char*)&item.first, sizeof(uint16_t));
				output.write((const char*)item.second.c_str(), 3);
			}

			output.close();
		}
		else
		{
			std::cout << "Couldn't write to highscores.bin";
		}
	}
}