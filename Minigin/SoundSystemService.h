#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "Singleton.h"

namespace engine 
{
	enum class SoundAction
	{
		Play,
		Stop
	};

	struct SoundEvent {
		SoundAction action;
		const std::string label;
		bool isMusic;
	};

	class SoundSystemService
	{
	public:
		SoundSystemService();
		virtual ~SoundSystemService();

		virtual void PlaySound(const std::string& soundLabel, bool isMusic);
		virtual void StopSound(const std::string& soundLabel);
		virtual void StopAllSound();

	private:
		class Impl;
		Impl* m_Impl;
	};

	class NullSoundSystemService final : public SoundSystemService
	{
	public:
		NullSoundSystemService() : SoundSystemService() {};
		~NullSoundSystemService() = default;

		virtual void PlaySound(const std::string&, bool ) override {};
		virtual void StopSound(const std::string& ) override {};
		virtual void StopAllSound() override {};
	};

	class LoggingSoundSystemService final : public SoundSystemService
	{
	public:
		LoggingSoundSystemService(std::unique_ptr<SoundSystemService> ss) : m_RealSS(std::move(ss)) {};
		virtual ~LoggingSoundSystemService() = default;

		virtual void PlaySound(const std::string& soundLabel, bool isMusic) override
		{
			m_RealSS->PlaySound(soundLabel, isMusic);
			std::cout << "Playing " << soundLabel << std::endl;
		};
		virtual void StopSound(const std::string& soundLabel) override
		{
			m_RealSS->StopSound(soundLabel);
			std::cout << "Stopped playing " << soundLabel << std::endl;
		};
		virtual void StopAllSound() override
		{
			std::cout << "Stopped all playing sounds\n";
		};

	private:
		std::unique_ptr<SoundSystemService> m_RealSS;
	};

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