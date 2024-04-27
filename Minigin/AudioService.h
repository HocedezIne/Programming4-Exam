#pragma once
#include <string>

namespace engine 
{
	class SoundSystemService final
	{
	public:
		SoundSystemService() = default;
		~SoundSystemService() = default;

		virtual void PlaySound(const std::string& /*soundLabel*/)
		{
			int test{};
			++test;
		};
		virtual void StopSound(const std::string& /*soundLabel*/) {};
		virtual void StopAllSound() {};
	};
}