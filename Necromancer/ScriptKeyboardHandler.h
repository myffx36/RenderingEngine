#pragma once

#include "KeyboardHandler.h"

namespace Necromancer{
	class Script;
	class ScriptKeyboardHandler:public KeyboardHandler{
	public:
		ScriptKeyboardHandler();
		virtual ~ScriptKeyboardHandler();
		virtual void handle_request(KeyboardEvent*);
	private:
		Script* m_script;
	};
}

