#pragma once

#include "Container.hpp"

namespace Necromancer{
	class Logger{
	public:
		~Logger();

		void Log(const String& str);
	private:
	};

	void Log(const String& str);
}

