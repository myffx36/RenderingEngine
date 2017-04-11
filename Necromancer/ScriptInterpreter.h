#pragma once

namespace Necromancer{
	class Configuration;
	class ScriptInterpreter{
	public:
		ScriptInterpreter(Configuration*);

		virtual ~ScriptInterpreter();
	private:
	};
}

