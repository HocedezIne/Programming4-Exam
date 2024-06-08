#include "GameStates.h"

int GameStateInterface::m_CurrentLevel = 1;
const int GameStateInterface::m_TotalLevels = 4;
GameMode GameStateInterface::m_GameMode = GameMode::Single;