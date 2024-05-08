#include "ServiceLocator.h"

namespace engine
{
	std::unique_ptr<SoundSystemServiceBase> ServiceLocator::audioserviceInstance = std::make_unique<NullSoundSystemService>();
}