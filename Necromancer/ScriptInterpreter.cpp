#include "stdafx.h"
#include "ScriptInterpreter.h"

#include "Configuration.h"
#include "WrapModules.h"
#include "InitModules.h"

namespace Necromancer{
	ScriptInterpreter::ScriptInterpreter(Configuration* configuration){
		init_wrap_modules();
		InitModules::init_modules();
		Py_Initialize();

		PyRun_SimpleString(
			"import sys\n"
			"sys.path.append('./script')\n"
			);
	}

	ScriptInterpreter::~ScriptInterpreter(){
	}
}