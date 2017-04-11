#include "stdafx.h"
#include "Logger.h"

#include "Necromancer.h"

namespace Necromancer{
	Logger::~Logger(){

	}

	void Log(const String& str){
		Necromancer* nec = Necromancer::get_instance();

		Logger* logger = nec->logger();

		logger->Log(str);
	}

	void Logger::Log(const String& str){

	}
}