#include "Observer.h"

namespace engine
{
	Subject::~Subject()
	{
		for (auto& observer : m_Observers)
		{
			if (observer) observer->RemoveSubject(this);
		}
	}

	void Subject::AddObserver(Observer* observer)
	{
		auto it = std::find(m_Observers.begin(), m_Observers.end(), observer);
		if (it == m_Observers.end())
		{
			m_Observers.push_back(observer);
			observer->AddSubject(this);
		}
	}

	void Subject::RemoveObserver(Observer* observer)
	{
		auto it = std::find(m_Observers.begin(), m_Observers.end(), observer);
		if (it != m_Observers.end())
		{
			m_Observers.erase(it);
			observer->RemoveSubject(this);
		}
	}

	void Subject::NotifyObservers(void* caller, Event event, const std::any& args)
	{
		for (auto& observer : m_Observers)
		{
			observer->OnNotify(caller, event, args);
		}
	}

	Observer::~Observer()
	{
		for (auto& subject : m_Subjects)
		{
			if (subject) subject->RemoveObserver(this);
		}
	}

	void Observer::AddSubject(Subject* subject)
	{
		auto it = std::find(m_Subjects.begin(), m_Subjects.end(), subject);
		if (it == m_Subjects.end()) m_Subjects.push_back(subject);
	}

	void Observer::RemoveSubject(Subject* subject)
	{
		auto it = std::find(m_Subjects.begin(), m_Subjects.end(), subject);
		if (it != m_Subjects.end()) m_Subjects.erase(it);
	}
}