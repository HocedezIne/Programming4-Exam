#pragma once
namespace engine
{
	template<typename Event>
	class ThreadSafeQueue {
	public:
		void enqueue(Event event) {
			m_Mutex.lock();
			m_Queue.push(event);
			m_Mutex.unlock();
			m_Condition.notify_one();
		}

		Event dequeue() {
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Condition.wait(lock, [this] {return !m_Queue.empty(); });
			Event event = m_Queue.front();
			m_Queue.pop();
			return event;
		}

	private:
		std::queue<Event> m_Queue;
		std::mutex m_Mutex;
		std::condition_variable m_Condition;
	};
}