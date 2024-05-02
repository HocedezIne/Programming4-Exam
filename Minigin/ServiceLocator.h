#pragma once
#include <memory>

#include "Singleton.h"
#include "SoundSystemService.h"

namespace engine
{
	class ServiceLocator final : public Singleton<ServiceLocator>
	{
	public:
		static SoundSystemService& GetSoundSystem() { 
			if (audioserviceInstance == nullptr)
			{
				std::cout << "No sound system registered. Using NullSoundSystem\n";
				RegisterSoundSystem(std::make_unique<engine::NullSoundSystemService>());
			}

			return *audioserviceInstance; 
		};

		static void RegisterSoundSystem(std::unique_ptr<SoundSystemService>&& ss) { audioserviceInstance = std::move(ss); };

	private:
		static std::unique_ptr<SoundSystemService> audioserviceInstance;
	};
}