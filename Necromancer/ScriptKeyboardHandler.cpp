#include "stdafx.h"
#include "ScriptKeyboardHandler.h"

#include "PILHIDInfoGetter.h"
#include "Script.h"

namespace Necromancer{
	ScriptKeyboardHandler::ScriptKeyboardHandler()
	{
		m_script = new Script("keyboard", "fun");
	}

	ScriptKeyboardHandler::~ScriptKeyboardHandler(){
		delete m_script;
	}

	void ScriptKeyboardHandler::handle_request(KeyboardEvent* event){
		(*m_script)(event);
	}
}