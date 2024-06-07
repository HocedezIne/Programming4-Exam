#include "GameStates.h"

int GameStateInterface::m_CurrentLevel = 1;
const int GameStateInterface::m_TotalLevels = 3;
GameMode GameStateInterface::m_GameMode = GameMode::Single;