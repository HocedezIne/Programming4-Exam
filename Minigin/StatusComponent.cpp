#include "StatusComponent.h"

namespace engine
{
	void engine::StatusComponent::AddDataMapping(const std::string& keyword, std::any value)
	{
		m_DataMap.insert(std::pair<std::string, std::any>(keyword, value));
	}

	std::any engine::StatusComponent::GetData(const std::string& keyword) const
	{
		auto index = m_DataMap.find(keyword);
		if (index == m_DataMap.end()) return NULL;

		return index->second;
	}

	void engine::StatusComponent::UpdateData(const std::string& keyword, std::any value)
	{
		auto index = m_DataMap.find(keyword);
		if (index != m_DataMap.end())
		{
			index->second = value;
		}
	}

	void engine::StatusComponent::RemoveDataMapping(const std::string& keyword)
	{
		m_DataMap.erase(keyword);
	}

	void engine::StatusComponent::OnNotify(Event event, void* /*caller*/, const std::any& args)
	{
		switch (event)
		{
		case Event::PlayerDied:
			UpdateData("LEFT", std::any_cast<int>(GetData("LEFT")) - 1);
			break;
		case Event::EnemyDied:
			UpdateData("SCORE", std::any_cast<int>(GetData("SCORE")) + std::any_cast<int>(args));
		default:
			break;
		}
	}
}