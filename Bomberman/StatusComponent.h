#pragma once
#include <map>
#include <string>
#include <any>

#include "Component.h"
#include "Observer.h"

class StatusComponent : public engine::Component, public engine::Observer
{
public:
	StatusComponent(engine::GameObject* owner) : engine::Component(owner) {};
	~StatusComponent() = default;
	StatusComponent(const StatusComponent& other) = delete;
	StatusComponent(StatusComponent&& other) = delete;
	StatusComponent& operator=(const StatusComponent& other) = delete;
	StatusComponent& operator=(StatusComponent&& other) = delete;

	void AddDataMapping(const std::string& keyword, std::any value);
	std::any GetData(const std::string& keyword) const;
	void UpdateData(const std::string& keyword, std::any value);
	void RemoveDataMapping(const std::string& keyword);

	virtual void OnNotify(engine::Event event, void* caller, const std::any& args) override;

private:
	std::map<std::string, std::any> m_DataMap;
};