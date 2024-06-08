#pragma once

#include "Singleton.h"
#include "GameObject.h"
#include "Observer.h"

class BombController : public engine::Singleton<BombController>, public engine::Observer
{
public:
	virtual void OnNotify(engine::Event event, void* caller, const std::any& args) override;

	void AddBomb(const glm::vec3 pos);
	void ExplodeAllBombs() { if (m_RemoteEnabled) for (auto bomb : m_Bombs) ExplodeBomb(bomb); };
	
	void ResetAllPowerups();
	void ResetRemote() { m_RemoteEnabled = false; };
	void ClearBombCount() { m_LiveBombs = 0; };

private:
	void ExplodeBomb(engine::GameObject* obj);

	bool m_RemoteEnabled{ false };

	const int m_GridSize{ 16 };
	const int m_DefaultMaxBombCount{ 1 };
	const int m_DefaultExplosionSize{ 1 };
	int m_MaxBombCount{ m_DefaultMaxBombCount };
	int m_LiveBombs{ 0 };
	int m_ExplosionSize{ m_DefaultExplosionSize };

	std::vector<engine::GameObject*> m_Bombs{};
};