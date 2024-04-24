#pragma once
#include <chrono>

#include "Singleton.h"

namespace engine
{
	class TimeService final : public Singleton<TimeService>
	{
	public:
		~TimeService() = default;

		void UpdateDeltaTime();

		const std::chrono::nanoseconds GetSleepTime() const;
		const float GetDeltaTime() const;


	private:
		const float m_TargetFrameTime{ 1.f/144.f };
		float m_DeltaTime{};

		std::chrono::steady_clock::time_point m_CurrentTime{};
		std::chrono::steady_clock::time_point m_PreviousTime{ std::chrono::high_resolution_clock::now() };
	};
}