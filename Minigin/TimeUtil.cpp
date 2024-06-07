#include "TimeUtil.h"

namespace engine 
{
	const float TimeUtil::m_TargetFrameTime = 1.f / 144.f;
	float TimeUtil::m_DeltaTime = 0.f;

	std::chrono::steady_clock::time_point TimeUtil::m_CurrentTime;
	std::chrono::steady_clock::time_point TimeUtil::m_PreviousTime = std::chrono::high_resolution_clock::now();

	void TimeUtil::UpdateDeltaTime()
	{
		m_CurrentTime = std::chrono::high_resolution_clock::now();
		m_DeltaTime = std::chrono::duration<float>(m_CurrentTime - m_PreviousTime).count();
		m_PreviousTime = m_CurrentTime;
	}

	const std::chrono::nanoseconds TimeUtil::GetSleepTime()
	{
		return (m_CurrentTime + std::chrono::milliseconds(static_cast<long long>(m_TargetFrameTime * 1000)) - std::chrono::high_resolution_clock::now());
	}

	const float TimeUtil::GetDeltaTime()
	{
		return m_DeltaTime;
	}

}