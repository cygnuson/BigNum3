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

#include <thread>
#include <chrono>

namespace cg {

/**Class for limiting the rate that a program can run run.

The speed limit will block with operator() untill a certian amount of time has
passed sense the last call to operator().

\tparam T The type of time unit to use. Must be some type of
std::chrono::duration. Must common is std::chrono::microseconds.
*/
template<typename T>
class SpeedLimit_Impl
{
public:
	/**Create a speed limiter
	\param t The minimum time between calls to oeprator()*/
	SpeedLimit_Impl(T t);
	/**Create a speed limiter with not set speed.*/
	SpeedLimit_Impl();
	/**Create a speed limiter with a set frame rate in frames per sec.
	\param frameRate The FPS that is needed.*/
	SpeedLimit_Impl(double frameRate);
	/**Set the speed limiter to cause minium frame rate.*/
	void FPS(double frameRate);
	/**Change the minimum amount of time between calls to operator()
	\param t The amount of time in std::chrono::duration*/
	template<typename U>
	void SetTime(U t);
	/**Activate the speed limit. If Time() in units of T (the template param)
	has not passed, the operator() will block untill that proper amount of time
	has passed.*/
	void operator()();
	/**Get the amount of time elapsed sense the SpeedLimit was created.*/
	std::size_t Elapsed() const;
	/**Increase the amount of time that is required to pass between calls to 
	operator().
	\param amt The amount to increase.*/
	template<typename U>
	void operator += (U amt);
	/**Decrease the amount of time that is required to pass between calls to
	operator().
	\param amt The amount to decrease.*/
	template<typename U>
	void operator -=(U amt);
	/**Set the diabled status.
	\param set The value of the DISABLED flag.  If true, the limiter will be
	disabled.*/
	void Disabled(bool set);
	/**Determine if the limiter is diabled.
	\return True if the limiter is disabled.*/
	bool Disabled() const;
private:
	/**The last time theat the operator() succeeded.*/
	std::chrono::high_resolution_clock::time_point m_lastTick;
	/**The time the object was created.*/
	std::chrono::high_resolution_clock::time_point m_initTime;
	/**The target frame rate.*/
	double m_expectedFrameRate = 0.0f;
	/**disable flag. if true,no blocking will happen.*/
	bool m_disabled = false;
	/**The time to wait between blocks.*/
	T m_time;
};

template<typename T>
inline cg::SpeedLimit_Impl<T>::SpeedLimit_Impl(T t)
{
	m_time = t;
	m_lastTick = std::chrono::high_resolution_clock().now();
	m_initTime = std::chrono::high_resolution_clock().now();
}

template<typename T>
inline SpeedLimit_Impl<T>::SpeedLimit_Impl()
{
	m_time = std::chrono::seconds(1);
	m_lastTick = std::chrono::high_resolution_clock().now();
	m_initTime = std::chrono::high_resolution_clock().now();
}

template<typename T>
inline SpeedLimit_Impl<T>::SpeedLimit_Impl(double frameRate)
{
	m_lastTick = std::chrono::high_resolution_clock().now();
	m_initTime = std::chrono::high_resolution_clock().now();
	FPS(frameRate);
}

template<typename T>
inline void SpeedLimit_Impl<T>::FPS(double frameRate)
{
	if (frameRate != 0)
	{
	m_expectedFrameRate = frameRate;
		double time = double(1) / m_expectedFrameRate;
		time *= 1e9;
		m_time = std::chrono::nanoseconds((uint64_t)std::floor(time));
	}
	else
	{
		m_time = std::chrono::seconds(1);
		m_disabled = true;
	}
}

template<typename T>
inline void SpeedLimit_Impl<T>::operator()()
{
	if (m_disabled)
		return;

	auto spent = (std::chrono::high_resolution_clock().now() - m_lastTick);
	if (spent < m_time)
	{
		auto sleepTime = m_time - spent;
		std::this_thread::sleep_for(sleepTime);
	}
	m_lastTick = std::chrono::high_resolution_clock().now();
}

template<typename T>
inline std::size_t SpeedLimit_Impl<T>::Elapsed() const
{
	return (std::chrono::duration_cast<std::chrono::seconds>
		(std::chrono::high_resolution_clock().now() - m_initTime)).count();
}

template<typename T>
inline void SpeedLimit_Impl<T>::Disabled(bool set)
{
	m_disabled = set;
}

template<typename T>
inline bool SpeedLimit_Impl<T>::Disabled() const
{
	return m_disabled;
}

template<typename T>
template<typename U>
inline void SpeedLimit_Impl<T>::SetTime(U t)
{
	m_time = std::chrono::duration_cast<T>(t);
	m_disabled = false;
}

template<typename T>
template<typename U>
inline void SpeedLimit_Impl<T>::operator+=(U amt)
{
	m_time += amt;
}

template<typename T>
template<typename U>
inline void SpeedLimit_Impl<T>::operator-=(U amt)
{
	m_time -= amt;
}

using SpeedLimit = SpeedLimit_Impl<std::chrono::nanoseconds>;


}