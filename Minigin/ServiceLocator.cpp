#include "ServiceLocator.h"

namespace engine
{
	std::unique_ptr<SoundSystemService> ServiceLocator::audioserviceInstance = std::make_unique<NullSoundSystemService>();
}