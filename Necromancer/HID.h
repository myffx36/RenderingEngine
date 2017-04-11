#pragma once

#include "HandlerUtility.hpp"
#include "CommonUtility.h"
#include "PILHIDInfoGetter.h"

namespace Necromancer{
	class Configuration;
	class PIL;
	class PILHIDInfoGetter;
	class HID{
	public:
		HID(Configuration*, PIL*);
		virtual ~HID();

		virtual void process_input_event();

		void attach_keyboard_handler(U32 i, KeyboardHandler*);
		void detach_keybaord_handler(U32 i);

		void attach_mouse_handler(U32 i, MouseHandler*);
		void detach_msoue_handler(U32 i);

		template<typename T>
		void attach_keyboard_fun(U32 i, bool pass_to_next, T fun){
			KeyboardHandler* keyboard_handler = 
				create_callable_keyboard_handler(fun, pass_to_next);
			attach_keyboard_handler(i, keyboard_handler);
		}
		template<typename T>
		void attach_mouse_fun(U32 i, bool pass_to_next, T fun){
			MouseHandler* mouse_handler = 
				create_callable_mouse_handler(fun, pass_to_next);
			attach_mouse_handler(i, mouse_handler);
		}

		void begin();
		void wait_for_end();
		void send_end_message();

		virtual void thread_main();
	private:
		static const U32 ks_slot_num = 10;
		KeyboardHandler* m_keyboard_handler[ks_slot_num];
		MouseHandler* m_mouse_handler[ks_slot_num];
		PILHIDInfoGetter* m_info_getter;

		std::thread m_thread;
		bool m_quit;
	};
}

