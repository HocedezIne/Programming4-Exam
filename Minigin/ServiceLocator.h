#pragma once
#include <memory>

#include "Singleton.h"
#include "SoundSystemService.h"

namespace engine
{
	class ServiceLocator final : public Singleton<ServiceLocator>
	{
	public:
		static SoundSystemService& GetSoundSystem() { return *audioserviceInstance; };
		static void RegisterSoundSystem(std::unique_ptr<SoundSystemService>&& ss) { audioserviceInstance = std::move(ss); };

	private:
		static std::unique_ptr<SoundSystemService> audioserviceInstance;
	};
}