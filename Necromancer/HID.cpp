#include "stdafx.h"
#include "HID.h"

#include "Configuration.h"
#include "PIL.h"
#include "PILHIDInfoGetter.h"
#include "Exception.h"
#include "STDWHIDInfoGetter.h"

namespace Necromancer{
	HID::HID(Configuration* config, PIL* pil){
		m_quit = false;
		m_info_getter = pil->pil_hid_info_getter();

		for(U32 i = 0;i < ks_slot_num;++ i){
			m_keyboard_handler[i] = nullptr;
			m_mouse_handler[i] = nullptr;
		}

		m_keyboard_handler[0] = create_callable_keyboard_handler(
			[](KeyboardEvent* event)
		{
		});
		m_mouse_handler[0] = create_callable_mouse_handler(
			[=](MouseEvent* event)
		{
		});
	}

	HID::~HID(){
		for(U32 i = 0;i < ks_slot_num;++ i){
			if(m_keyboard_handler[i]) delete m_keyboard_handler[i];
			if(m_mouse_handler[i]) delete m_mouse_handler[i];
		}
	}

	void HID::process_input_event(){
		const U32 max_event_number = 1024;
		KeyboardEvent keyboard_event[max_event_number];
		U32 keyboard_event_number;
		MouseEvent mouse_event[max_event_number];
		U32 mouse_event_number;
		m_info_getter->get_keyboard_event(keyboard_event, keyboard_event_number);
		m_info_getter->get_mouse_event(mouse_event, mouse_event_number);

		for(U32 i = 0;i < keyboard_event_number;++ i){
			m_keyboard_handler[0]->handle_request(&keyboard_event[i]);
		}
		for(U32 i = 0;i < mouse_event_number;++ i){
			m_mouse_handler[0]->handle_request(&mouse_event[i]);
		}
	}

	void HID::attach_keyboard_handler(U32 i, KeyboardHandler* handler){
#ifdef _DEBUG
		if((i <= 0)||(i >= 9)){
			throw Exception("attach_keyboard_handler: index out of range");
		}
#endif

		if(m_keyboard_handler[i]) delete m_keyboard_handler[i];
		for(U32 j = i - 1;j >= 0;-- j){
			if(m_keyboard_handler[j]){
				m_keyboard_handler[j]->set_successor(handler);
				break;
			}
		}

		handler->set_successor(nullptr);

		for(U32 j = i + 1;j < ks_slot_num;++ j){
			if(m_keyboard_handler[j]){
				handler->set_successor(m_keyboard_handler[j]);
				break;
			}
		}

		m_keyboard_handler[i] = handler;
	}

	void HID::detach_keybaord_handler(U32 i){
#ifdef _DEBUG
		if((i <= 0)||(i >= 9)){
			throw Exception("detach_keybaord_handler: index out of range");
		}
#endif

		if(m_keyboard_handler[i]) delete m_keyboard_handler[i];
		m_keyboard_handler[i] = nullptr;
		KeyboardHandler* pre = nullptr, *post = nullptr;
		for(U32 j = i - 1;j >= 0;-- j){
			if(m_keyboard_handler[j]){
				pre = m_keyboard_handler[j];
				break;
			}
		}
		for(U32 j = i + 1;j < ks_slot_num;++ j){
			if(m_keyboard_handler[j]){
				post = m_keyboard_handler[j];
				break;
			}
		}
		pre->set_successor(post);

		m_keyboard_handler[i] = nullptr;
	}

	void HID::attach_mouse_handler(U32 i, MouseHandler* handler){
#ifdef _DEBUG
		if((i <= 0)||(i >= 9)){
			throw Exception("attach_mouse_handler: index out of range");
		}
#endif

		if(m_mouse_handler[i]) delete m_mouse_handler[i];
		for(U32 j = i - 1;j >= 0;-- j){
			if(m_mouse_handler[j]){
				m_mouse_handler[j]->set_successor(handler);
				break;
			}
		}

		handler->set_successor(nullptr);

		for(U32 j = i + 1;j < ks_slot_num;++ j){
			if(m_mouse_handler[j]){
				handler->set_successor(m_mouse_handler[j]);
				break;
			}
		}
		m_mouse_handler[i] = handler;
	}

	void HID::detach_msoue_handler(U32 i){
#ifdef _DEBUG
		if((i <= 0)||(i >= 9)){
			throw Exception("detach_msoue_handler: index out of range");
		}
#endif

		if(!m_mouse_handler[i]) return;
		m_mouse_handler[i] = nullptr;
		MouseHandler* pre = nullptr, *post = nullptr;
		for(U32 j = i - 1;j >= 0;-- j){
			if(m_mouse_handler[j]){
				pre = m_mouse_handler[j];
				break;
			}
		}
		for(U32 j = i + 1;j < ks_slot_num;++ j){
			if(m_mouse_handler[j]){
				post = m_mouse_handler[j];
				break;
			}
		}
		pre->set_successor(post);
		m_mouse_handler[i] = nullptr;
	}

	void HID::begin(){
		m_thread = std::move(std::thread(&::Necromancer::HID::thread_main, this));
	}

	void HID::send_end_message(){
		m_quit = true;
	}

	void HID::wait_for_end(){
		m_thread.join();
	}

	void HID::thread_main(){
		const U32 max_event_number = 1024;
		MouseEvent mouse_event[max_event_number];
		KeyboardEvent keyboard_event[max_event_number];
		while(!m_quit){
			U32 keyboard_event_number;
			U32 mouse_event_number;
			m_info_getter->get_keyboard_event(keyboard_event, keyboard_event_number);
			m_info_getter->get_mouse_event(mouse_event, mouse_event_number);

			for(U32 i = 0;i < keyboard_event_number;++ i){
				m_keyboard_handler[0]->handle_request(&keyboard_event[i]);
			}
			for(U32 i = 0;i < mouse_event_number;++ i){
				m_mouse_handler[0]->handle_request(&mouse_event[i]);
			}

			STDWHIDInfoGetter* stdw_hg = 
				dynamic_cast<STDWHIDInfoGetter*>(m_info_getter);
			if(keyboard_event_number == 0 && 
				mouse_event_number == 0 &&
				stdw_hg != nullptr)
			{
				std::promise<int> t_promise;
				std::promise<int>& promise = 
					std::ref(stdw_hg->set_promise(std::move(t_promise)));
				promise.get_future().wait();
			}
		}
	}
}