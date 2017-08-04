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

#pragma once


#include <chrono>
#include <functional>
#include <thread>
#include <cmath>

#include "SpeedLimit.hpp"

namespace cg {
/**Generic timing fucntions.
*
*The timer starts when its created.
*/
class Timer
{
public:

	Timer(bool startPaused = false);
	std::chrono::nanoseconds Restart();
	/**Get the time in seconds that passed sense the objects creation.*/
	template<typename DurationType = std::chrono::duration<double>>
	DurationType GetTime();
	/**Pause the timer.*/
	void Pause();
	/**Unpause the timer.*/
	void UnPause();
	/**get the chrono duration.
	\return The time passed as a chrono duration.*/
	std::chrono::nanoseconds GetDuration() const;
	/*start the timer.*/
	void Start();
	/**Call a callable object FPS times per second.  The caller should return
	false when its time to stop running.
	\param fps the calls per second.  If FPS is zero, there will be no limit.
	\param caller The callable object to call FPS calls per sec.  The caller
	should return false when its time to stop running.
	\param amt The max amount of calls to make.
	\param args The arguments to call the caller with.*/
	template<typename Call, typename...Args>
	static void CallPerSec(double fps,
		uint64_t amt,
		Call && caller,
		Args&&...args);
	/**Call a callable object X amount of times over Y amount of durations.
	\param caller The callable object to call. when the caller returns false,
	the process stops.
	\param amt The amount of times to call the caller.
	\param duration The std::chrono::duration that it will take to call the
	caller amt times.
	\param args The args that go to the caller.*/
	template<typename Call, typename...Args>
	static void CallOverTime(uint64_t amt,
		std::chrono::nanoseconds duration,
		Call&& caller,
		Args&&...args);
	/**Execute a callable in a timed fasion.
	\param caller The callable to time.
	\param args The args to send to the caller.
	\tparam TimeUnit an std::chrono::duration that will be returned.
	\return The amount of time the execution took.*/
	template<typename TimeUnit = std::chrono::duration<float>,
		typename Caller, typename...Args>
	static TimeUnit TimedCall(Caller& caller,
		Args...args);
	/**Translate any chrono::duration premade unit to a string.*/
	template<typename T>
	static std::string TranslateTimeUnit();
private:
	/**The time that the timer started.*/
	std::chrono::high_resolution_clock::time_point t1;
	/**The time that the timer paused.*/
	std::chrono::high_resolution_clock::time_point m_pausedTime;
	/**The amount of paused time.*/
	std::chrono::nanoseconds m_pausedAmount;
	/**currently paused or not.*/
	bool m_paused;
};

template<typename DurationType>
inline DurationType Timer::GetTime()
{
	return std::chrono::duration_cast<DurationType>(GetDuration());
}

template<typename Call, typename...Args>
inline void Timer::CallPerSec(double fps,
	uint64_t amt,
	Call && caller,
	Args&&...args)
{
	SpeedLimit sl;
	sl.FPS(fps);
	bool run = true;
	uint64_t calls = 0;
	while (run)
	{
		sl();
		run = caller(std::forward<Args>(args)...)
			&& ++calls <= (amt-1);
	}
}

template<typename Call, typename ...Args>
inline void Timer::CallOverTime(uint64_t amt,
	std::chrono::nanoseconds duration,
	Call&& caller,
	Args&&...args)
{
	uint64_t nanoPerCall = duration.count() / amt;
	SpeedLimit sl((std::chrono::nanoseconds(nanoPerCall)));
	bool run = true;
	uint64_t calls = 0;
	while (run)
	{
		sl();
		run = caller(std::forward<Args>(args)...)
			&& ++calls <= (amt - 1);
	}
}

template<typename TimeUnit, typename Caller, typename ...Args>
inline TimeUnit Timer::TimedCall(Caller & caller, 
	Args ...args)
{
	Timer timer;
	timer.Start();
	caller(std::forward<Args>(args)...);
	return std::chrono::duration_cast<TimeUnit>(timer.GetDuration());
}

template<typename T>
inline std::string cg::Timer::TranslateTimeUnit()
{
	if (std::is_same<T, std::chrono::nanoseconds>::value)
		return "NanoSeconds";
	else if (std::is_same<T, std::chrono::microseconds>::value)
		return "MicroSeconds";
	else if (std::is_same<T, std::chrono::milliseconds>::value)
		return "MilliSeconds";
	else if (std::is_same<T, std::chrono::seconds>::value)
		return "Seconds";
	else if (std::is_same<T, std::chrono::minutes>::value)
		return "Minutes";
	else if (std::is_same<T, std::chrono::minutes>::value)
		return "HoursSeconds";

	return "Unknown";
}




}

