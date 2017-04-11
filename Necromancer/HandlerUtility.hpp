#ifndef NECROMANCER_HANDLER_UTILITY_HPP
#define NECROMANCER_HANDLER_UTILITY_HPP

#include "KeyboardHandler.h"
#include "MouseHandler.h"

namespace Necromancer{
	template<typename T>
	class KeyboardCallableHandler:public KeyboardHandler{
	public:
		KeyboardCallableHandler(T caller, bool pass_to_next = true)
			:m_caller(caller), m_pass_to_next(pass_to_next)
		{
		}
		virtual void handle_request(KeyboardEvent* event){
			m_caller(event);
			if(m_pass_to_next) pass_to_next(event);
		}
	private:
		T m_caller;
		bool m_pass_to_next;
	};
	template<typename T>
	KeyboardHandler* create_callable_keyboard_handler(T caller, 
		bool pass_to_next = true)
	{
		return new KeyboardCallableHandler<T>(caller, pass_to_next);
	}

	template<typename T>
	class MouseCallableHandler:public MouseHandler{
	public:
		MouseCallableHandler(T caller, bool pass_to_next = true)
			:m_caller(caller), m_pass_to_next(pass_to_next)
		{
		}
		virtual void handle_request(MouseEvent* event){
			m_caller(event);
			if(m_pass_to_next) pass_to_next(event);
		}
	private:
		T m_caller;
		bool m_pass_to_next;
	};
	template<typename T>
	MouseHandler* create_callable_mouse_handler(T caller, 
		bool pass_to_next = true)
	{
		return new MouseCallableHandler<T>(caller, pass_to_next);
	}
}

#endif