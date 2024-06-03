#pragma once

#include "Singleton.h"
#include "GameObject.h"
#include "Observer.h"

class BombController : public engine::Singleton<BombController>, public engine::Observer
{
public:
	virtual void OnNotify(engine::Event event, void* caller, const std::any& args) override;

	void AddBomb(const glm::vec3 pos);

private:
	const int m_GridSize{ 16 };
	int m_MaxBombCount{ 1 };
	int m_LiveBombs{ 0 };
	int m_ExplosionSize{ 1 };
};