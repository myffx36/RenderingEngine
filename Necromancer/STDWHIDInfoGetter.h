#pragma once

#include "PILHIDInfoGetter.h"
#include "CommonUtility.h"

#include "Container.hpp"

namespace Necromancer{
	struct WinMessage{
		UINT message;
		WPARAM w_param;
		LPARAM l_param;
	};

	class STDWHIDInfoGetter:public PILHIDInfoGetter{
	public:
		STDWHIDInfoGetter();
		virtual ~STDWHIDInfoGetter();

		virtual void get_keyboard_event(KeyboardEvent*, U32& num);
		virtual void get_mouse_event(MouseEvent*, U32& num);

		void buffer_keyboard_event(UINT message, WPARAM wParam, LPARAM lParam);

		void buffer_mouse_event(UINT message, WPARAM wParam, LPARAM lParam);

		std::promise<int>& set_promise(std::promise<int>&& promise);
	private:
		KeyInfo wink_to_key_info(unsigned long x);
		KeyboardEvent convert_to_nm_keyboard_event(WinMessage);
	private:
		Queue<WinMessage>::Type m_keyboard_message;
		Queue<WinMessage>::Type m_mouse_message;
		std::mutex m_mutex, m_promise_mutex;
		std::promise<int> m_promise;
	};
}

