#include "stdafx.h"
#include "KeyboardHandler.h"

namespace Necromancer{
	KeyboardHandler::KeyboardHandler(){
		m_successor = nullptr;
	}

	KeyboardHandler::~KeyboardHandler(){
	}

	void KeyboardHandler::handle_request(KeyboardEvent* event){
		do_handle_request(event);

		pass_to_next(event);
	}

	void KeyboardHandler::pass_to_next(KeyboardEvent* event){
		if(m_successor) m_successor->handle_request(event);
	}

	void KeyboardHandler::set_successor(KeyboardHandler* successor){
		m_successor = successor;
	}

	KeyboardHandler* KeyboardHandler::successor(){
		return m_successor;
	}

	void KeyboardHandler::do_handle_request(KeyboardEvent* event){

	}
}