#include "StatusComponent.h"

namespace engine
{
	void engine::StatusComponent::AddDataMapping(const std::string& keyword, int value)
	{
		m_DataMap.insert(std::pair<std::string, int>(keyword, value));
	}

	int engine::StatusComponent::GetData(const std::string& keyword) const
	{
		auto index = m_DataMap.find(keyword);
		if (index == m_DataMap.end()) return NULL;

		return index->second;
	}

	void engine::StatusComponent::UpdateData(const std::string& keyword, int value)
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

	void engine::StatusComponent::OnNotify(void* caller, Event event, const std::any& args)
	{
		caller;

		switch (event)
		{
		case Event::PlayerDied:
			UpdateData("Lives", GetData("Lives") - 1);
			break;
		case Event::EnemyDied:
			UpdateData("Score", GetData("Score") + std::any_cast<int>(args));
		default:
			break;
		}
	}
}