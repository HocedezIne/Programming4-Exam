#include "EnemyStateInterface.h"

#include <algorithm>

#include "CollisionSystem.h"

EnemyStateInterface::EnemyStateInterface(float speed, engine::GameObject* enemy, 
	const std::vector<engine::GameObject*> players, float chasingDistance) :
	m_Speed(speed), m_ChasingDistance(chasingDistance), m_EnemyObject(enemy), m_Players(players)
{
	collisionSystem::collisionHandler.AddObserver(this);

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

	m_MoveCommand = std::make_unique<engine::MoveInputCommand>(m_EnemyObject, m_Direction, m_Speed);
}

void EnemyStateInterface::ChangeDirection()
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

	//m_EnemyObject->SetLocalPosition(m_EnemyObject->GetPreviousLocalPosition());
	m_MoveCommand = std::make_unique<engine::MoveInputCommand>(m_EnemyObject, m_Direction, m_Speed);
}

void EnemyStateInterface::ChangeDirection(const glm::vec3 direction)
{
	m_Direction = direction;
	m_EnemyObject->SetLocalPosition(m_EnemyObject->GetPreviousLocalPosition());
	m_MoveCommand = std::make_unique<engine::MoveInputCommand>(m_EnemyObject, m_Direction, m_Speed);
}

void EnemyStateInterface::OnNotify(engine::Event event, void* caller, const std::any& /*args*/)
{
	if (event == engine::Event::EnemyWalkedIntoWall)
	{
		auto obj = static_cast<engine::GameObject*>(caller);

		if (m_EnemyObject == obj)
		{
			auto pos = obj->GetLocalPosition();
			pos.x = std::roundf(pos.x / 16.f) * 16.f;
			pos.y = std::roundf(pos.y / 16.f) * 16.f;

			obj->SetLocalPosition(pos);

			ChangeDirection();
		}
	}
}

void EnemyStateInterface::OnExit()
{
	collisionSystem::collisionHandler.RemoveObserver(this);
}

EnemyStateWalking::EnemyStateWalking(float speed, engine::GameObject* enemy, 
	const std::vector<engine::GameObject*> players, float chasingDistance) :
	EnemyStateInterface(speed, enemy, players, chasingDistance)
{}

void EnemyStateWalking::Update()
{
	m_MoveCommand->Execute();
}

void EnemyStateWalking::OnEnter()
{
}

void EnemyStateWalking::OnExit()
{

}

EnemyStateChasing::EnemyStateChasing(float speed, engine::GameObject* enemy, 
	const std::vector<engine::GameObject*> players, float chasingDistance, int index) :
	EnemyStateInterface(speed, enemy, players, chasingDistance), m_IndexToChase(index)
{}

void EnemyStateChasing::Update()
{
	glm::vec3 newDirection = m_Players[m_IndexToChase]->GetLocalPosition()- m_EnemyObject->GetLocalPosition();

	if (abs(newDirection.x) > abs(newDirection.y))
	{
		newDirection.y = 0.f;
		newDirection.x = (newDirection.x > 0) ? 1.f : -1.f;
	}
	else
	{
		newDirection.x = 0.f;
		newDirection.y = (newDirection.y > 0) ? 1.f : -1.f;
	}
	newDirection.z = 0.f;

	ChangeDirection(newDirection);
	m_MoveCommand->Execute();
}

void EnemyStateChasing::OnEnter()
{
}

void EnemyStateChasing::OnExit()
{

}