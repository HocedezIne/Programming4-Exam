#include "ServiceLocator.h"

namespace engine
{
	std::unique_ptr<SoundSystemService> ServiceLocator::audioserviceInstance;
}