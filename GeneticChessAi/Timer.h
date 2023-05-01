#pragma once
#include <chrono>

class Timer
{
public:
	Timer();

	void Start();

	template<typename TimeScale>
	float GetDuration();


private:
	std::chrono::high_resolution_clock::time_point m_startPoint;

};

template<typename TimeScale>
float Timer::GetDuration()
{
	std::chrono::duration<float, TimeScale> duration = (std::chrono::high_resolution_clock::now() - m_startPoint);
	return duration.count();
}


