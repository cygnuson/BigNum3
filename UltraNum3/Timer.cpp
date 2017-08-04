/*

(C) Matthew Swanson

This file is part of UltraNum2.

UltraNum2 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

UltraNum2 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with UltraNum2.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Timer.hpp"

namespace cg {

Timer::Timer(bool startPaused)
{
	m_pausedAmount = std::chrono::nanoseconds(0);
	if (startPaused)
	{
		Start();
		Pause();
	}
	else
	{
		m_paused = false;
		Start();
	}

}
inline void Timer::Start()
{
	t1 = std::chrono::high_resolution_clock::now();
}

void Timer::Pause()
{
	if (!m_paused)
	{
		m_pausedTime = std::chrono::high_resolution_clock::now();
		m_paused = true;
	}
}

void Timer::UnPause()
{
	if (m_paused)
	{
		m_pausedAmount +=
			(std::chrono::high_resolution_clock::now() - m_pausedTime);
		m_paused = false;
	}
}

std::chrono::nanoseconds Timer::GetDuration() const
{
	auto end = std::chrono::high_resolution_clock::now();
	auto timeInNSec = (end - t1);
	auto pausedAmt = m_pausedAmount;
	if (m_paused)
	{
		pausedAmt +=
			(std::chrono::high_resolution_clock::now() - m_pausedTime);
	}
	return timeInNSec - pausedAmt;
}
// restart the timer.
std::chrono::nanoseconds  Timer::Restart() {
	auto t = GetDuration();
	Start();
	return t;
}

}