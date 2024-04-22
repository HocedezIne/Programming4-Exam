#include "TimeService.h"

namespace engine 
{
	void TimeService::UpdateDeltaTime()
	{
		m_CurrentTime = std::chrono::high_resolution_clock::now();
		m_DeltaTime = std::chrono::duration<float>(m_CurrentTime - m_PreviousTime).count();
		m_PreviousTime = m_CurrentTime;
	}

	const std::chrono::nanoseconds TimeService::GetSleepTime() const
	{
		return (m_CurrentTime + std::chrono::milliseconds(static_cast<long long>(m_TargetFrameTime * 1000)) - std::chrono::high_resolution_clock::now());
	}

	const float TimeService::GetDeltaTime() const
	{
		return m_DeltaTime;
	}

}