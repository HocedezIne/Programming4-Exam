#include "EnemyStateInterface.h"

#include <algorithm>

#include "CollisionSystem.h"

EnemyStateWalking::EnemyStateWalking(float speed, engine::GameObject* enemy) :
	EnemyStateInterface(), m_Speed(speed), m_EnemyObject(enemy)
{
	srand(static_cast<unsigned int>(time(NULL)));

	int randomIndex = rand() % 4;

	switch (randomIndex) {
	case 0:
		m_Direction = glm::vec3(1.f, 0.f, 0.f);
		break;
	case 1:
		m_Direction = glm::vec3(-1.f, 0.f, 0.f);
		break;
	case 2:
		m_Direction = glm::vec3(0.f, 1.f, 0.f);
		break;
	case 3:
		m_Direction = glm::vec3(0.f, -1.f, 0.f);
		break;
	}
}

void EnemyStateWalking::Update()
{
	m_MoveCommand->Execute();
}

void EnemyStateWalking::OnEnter()
{
	collisionSystem::collisionHandler.AddObserver(this);
	ChangeDirection();
}

void EnemyStateWalking::OnExit()
{

}

void EnemyStateWalking::OnNotify(engine::Event event, void* caller, const std::any& /*args*/)
{
	if (event == engine::Event::EnemyWalkedIntoWall)
	{
		auto obj = static_cast<engine::GameObject*>(caller);
		if (m_EnemyObject == obj)
			ChangeDirection();
	}
}

void EnemyStateWalking::ChangeDirection()
{
	int idx{ rand() % 4 };
	switch (idx)
	{
	case 0:
	case 2:
		m_Direction *= -1;
		break;
	case 1:
		m_Direction *= -1;
	case 3: 
		std::swap(m_Direction.x, m_Direction.y);
		break;
	default:
		break;
	}

	m_EnemyObject->SetLocalPosition(m_EnemyObject->GetPreviousLocalPosition());
	m_MoveCommand = std::make_unique<engine::MoveInputCommand>(m_EnemyObject, m_Direction, m_Speed);
}