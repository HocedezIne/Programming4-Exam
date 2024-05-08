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
		Stop,
		Quit
	};

	struct SoundEvent {
		SoundAction action;
		const std::string label;
		bool isMusic;
	};

	class SoundSystemServiceBase
	{
	public:
		SoundSystemServiceBase() = default;
		virtual ~SoundSystemServiceBase() = default;

		virtual void PlaySound(const std::string& soundLabel, bool isMusic) = 0;
		virtual void StopSound(const std::string& soundLabel) = 0;
		virtual void StopAllSound() = 0;
	};

	class SoundSystemService final : public SoundSystemServiceBase
	{
	public:
		SoundSystemService();
		~SoundSystemService();

		virtual void PlaySound(const std::string& soundLabel, bool isMusic) override;
		virtual void StopSound(const std::string& soundLabel) override;
		virtual void StopAllSound() override;

	private:
		class Impl;
		Impl* m_Impl;
	};

	class NullSoundSystemService final : public SoundSystemServiceBase
	{
	public:
		NullSoundSystemService() : SoundSystemServiceBase() {};
		virtual ~NullSoundSystemService() = default;

		virtual void PlaySound(const std::string&, bool ) override {};
		virtual void StopSound(const std::string& ) override {};
		virtual void StopAllSound() override {};
	};

	class LoggingSoundSystemService final : public SoundSystemServiceBase
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
			m_RealSS->StopAllSound();
			std::cout << "Stopped all playing sounds\n";
		};

	private:
		std::unique_ptr<SoundSystemService> m_RealSS;
	};
}