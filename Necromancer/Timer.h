#pragma once

#include "CommonUtility.h"

namespace Necromancer{
	class Timer{
	public:
		static U32 get_now();
		Timer();
		F64 time();

		F32 get_dlt_time();
		void init();
	private:
		F64 get_current_time();
		F64 m_start_time, m_current_time;
		F32 m_last_time;
	};
}

