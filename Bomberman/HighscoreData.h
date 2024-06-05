#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>

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
			std::cout << "Couldn't read highscore.bin\n";
		}

		std::ostringstream result;
		result << std::setw(2) << std::setfill('0') << std::to_string(highscore);
		return result.str();
	}

	auto GetLeaderboardData()
	{
		std::vector<std::pair<uint16_t, std::string>> result;

		if (std::ifstream input{ "../Data/Highscores.bin", std::ios::binary }; input.is_open())
		{
			for (size_t idx{}; idx < 5; ++idx)
			{
				uint16_t score;
				input.read((char*)&score, sizeof(score));
				std::string name;
				input.read((char*)&name, 4); // name length is always 3 + null terminator

				result.emplace_back( std::make_pair(score, name) );
			}

			input.close();
		}
		else
		{
			std::cout << "Couldn't read highscore.bin\n";
		}

		return result;
	}

	void SaveLeaderboardData(std::vector<std::pair<uint16_t, std::string>> data)
	{
		if (std::ofstream output{ "../Data/Highscores.bin", std::ios::binary }; output.is_open())
		{
			for (auto item : data)
			{
				output.write((const char*)&item.first, sizeof(uint16_t));
				output.write((const char*)item.second.c_str(), 4);
			}

			output.close();
		}
		else
		{
			std::cout << "Couldn't write to highscore.bin";
		}
	}
}