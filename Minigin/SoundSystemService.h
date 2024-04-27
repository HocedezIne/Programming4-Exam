#pragma once

#include <string>
#include <memory>
#include <iostream>

#include "Singleton.h"

namespace engine 
{
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
}