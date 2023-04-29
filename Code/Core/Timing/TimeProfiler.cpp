#include "JupiterPCH.h"
#include "TimeProfiler.h"

namespace jpt
{
#if IS_PROFILING
	TimeProfiler::TimeProfiler(const char* pLabel)
	{
		//JPT_UNUSED(pLabel);
		m_pLabel = pLabel;
		m_timer.Start();
	}

	jpt::TimeProfiler::~TimeProfiler()
	{
		const TimingPrecision duration = m_timer.GetTimeDuration();
		JPT_SYSTEM_INFO("%s: %.2f ms", m_pLabel, duration);
	}
#endif
}