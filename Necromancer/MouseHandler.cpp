#include "stdafx.h"
#include "MouseHandler.h"

namespace Necromancer{
	MouseHandler::MouseHandler(){
		m_successor = nullptr;
	}

	MouseHandler::~MouseHandler(){
	}

	void MouseHandler::handle_request(MouseEvent* event){
		do_handle_request(event);

		pass_to_next(event);
	}

	void MouseHandler::pass_to_next(MouseEvent* event){
		if(m_successor) m_successor->handle_request(event);
	}

	void MouseHandler::set_successor(MouseHandler* successor){
		m_successor = successor;
	}

	MouseHandler* MouseHandler::successor(){
		return m_successor;
	}

	void MouseHandler::do_handle_request(MouseEvent* event){

	}

}