#pragma once
#include <map>
#include <string>
#include <any>

#include "Component.h"
#include "Observer.h"

namespace engine
{
	class StatusComponent : public Component, public Observer
	{
	public:
		StatusComponent(GameObject* owner) : Component(owner) {};
		~StatusComponent() = default;
		StatusComponent(const StatusComponent& other) = delete;
		StatusComponent(StatusComponent&& other) = delete;
		StatusComponent& operator=(const StatusComponent& other) = delete;
		StatusComponent& operator=(StatusComponent&& other) = delete;

		void AddDataMapping(const std::string& keyword, int value);
		int GetData(const std::string& keyword) const;
		void UpdateData(const std::string& keyword, int value);
		void RemoveDataMapping(const std::string& keyword);

		virtual void OnNotify(void* caller, Event event, const std::any& args) override;

	private:
		std::map<std::string, int> m_DataMap;
	};
}