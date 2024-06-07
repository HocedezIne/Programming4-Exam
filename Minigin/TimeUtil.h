#pragma once
#include <chrono>

#include "Singleton.h"

namespace engine
{
	class TimeUtil final
	{
	public:
		~TimeUtil() = default;

		static void UpdateDeltaTime();

		static const std::chrono::nanoseconds GetSleepTime();
		static const float GetDeltaTime();


	private:
		TimeUtil() = default;

		static const float m_TargetFrameTime;
		static float m_DeltaTime;

		static std::chrono::steady_clock::time_point m_CurrentTime;
		static std::chrono::steady_clock::time_point m_PreviousTime;
	};
}