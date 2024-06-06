#pragma once
#include <vector>
#include <any>

namespace engine
{
	enum class Event
	{
		PlayerDied,
		EnemyDied,
		TimerFinished,
		PlayerOnExit,
		EnemyWalkedIntoWall,
		PowerUpCollected
	};

	class Subject;
	class Observer
	{
	public:
		virtual ~Observer();

		virtual void OnNotify(Event event, void* caller, const std::any& args) = 0;

	private:
		friend class Subject;
		void AddSubject(Subject* subject);
		void RemoveSubject(Subject* subject);

		std::vector<Subject*> m_Subjects{ nullptr };
	};

	class Subject
	{
	public:
		Subject() = default;
		virtual ~Subject();

		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);

	protected:
		void NotifyObservers(Event event, void* caller, const std::any& args);

	private:
		std::vector<Observer*> m_Observers;
	};
}