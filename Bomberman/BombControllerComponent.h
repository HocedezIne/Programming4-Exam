#pragma once

#include "Component.h"
#include "GameObject.h"
#include "Observer.h"

class BombControllerComponent : public engine::Component, public engine::Observer
{
public:
	BombControllerComponent(engine::GameObject* owner) : engine::Component(owner)
	{};
	virtual ~BombControllerComponent() = default;

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