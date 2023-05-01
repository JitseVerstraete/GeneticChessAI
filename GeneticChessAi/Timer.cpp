#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
	Start();
}

void Timer::Start()
{
	m_startPoint = high_resolution_clock::now();
}

