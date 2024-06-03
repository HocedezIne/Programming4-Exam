#include "StatusComponent.h"

void StatusComponent::AddDataMapping(const std::string& keyword, std::any value)
{
	m_DataMap.insert(std::pair<std::string, std::any>(keyword, value));
}

std::any StatusComponent::GetData(const std::string& keyword) const
{
	auto index = m_DataMap.find(keyword);
	if (index == m_DataMap.end()) return NULL;

	return index->second;
}

void StatusComponent::UpdateData(const std::string& keyword, std::any value)
{
	auto index = m_DataMap.find(keyword);
	if (index != m_DataMap.end())
	{
		index->second = value;
	}
}

void StatusComponent::RemoveDataMapping(const std::string& keyword)
{
	m_DataMap.erase(keyword);
}

void StatusComponent::OnNotify(engine::Event event, void* /*caller*/, const std::any& args)
{
	switch (event)
	{
	case engine::Event::PlayerDied:
		UpdateData("LEFT", std::any_cast<int>(GetData("LEFT")) - 1);
		break;
	case engine::Event::EnemyDied:
		UpdateData("SCORE", std::any_cast<int>(GetData("SCORE")) + std::any_cast<int>(args));
		break;
	default:
		break;
	}
}