#include "stdafx.h"
#include "Timer.h"

#ifdef _WIN32
#endif

namespace Necromancer{

	Timer::Timer(){
		m_start_time = get_current_time();
		m_last_time = 0.0f;
	}

	F64 Timer::time(){
		m_current_time = get_current_time();
		return m_current_time - m_start_time;
	}

	F64 Timer::get_current_time(){
#ifdef _WIN32
		static LARGE_INTEGER count, frequency;
		QueryPerformanceCounter(&count);
		QueryPerformanceFrequency(&frequency);
		static F64 x, y;
		x = static_cast<double>(count.QuadPart);
		y = static_cast<double>(frequency.QuadPart);
		return (x / y);
#endif
	}

	
	U32 Timer::get_now(){
		auto now = std::chrono::high_resolution_clock::now();

		std::chrono::milliseconds output = 
			std::chrono::duration_cast<
			std::chrono::milliseconds>(now.time_since_epoch());

		return (U32)output.count();
	}


	F32 Timer::get_dlt_time(){
		if (m_last_time == 0.0f){
			m_last_time = (F32)get_current_time();
			return 0.0f;
		}
		else{
			F32 current = (F32)get_current_time();
			F32 dlt = current - m_last_time;
			m_last_time = current;
			return dlt;
		}
	}
}