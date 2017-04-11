#include "stdafx.h"
#include "STDWHIDInfoGetter.h"

namespace Necromancer{
	namespace{
		STDWHIDInfoGetter* instance;
	}

	STDWHIDInfoGetter::STDWHIDInfoGetter(){
		instance = this;
	}

	STDWHIDInfoGetter::~STDWHIDInfoGetter(){
		instance = nullptr;
	}

	void STDWHIDInfoGetter::get_keyboard_event(KeyboardEvent* events, U32& num){
		num = static_cast<U32>(m_keyboard_message.size());
		int k = 0;
		while(!m_keyboard_message.empty()){
			m_mutex.lock();
			WinMessage win_message = m_keyboard_message.front();
			m_keyboard_message.pop();
			m_mutex.unlock();
			events[k++] = convert_to_nm_keyboard_event(win_message);
		}
	}

	void STDWHIDInfoGetter::get_mouse_event(MouseEvent* events, U32& num){
		m_mutex.lock();
		num = 0;
		m_mutex.unlock();
	}

	KeyboardEvent STDWHIDInfoGetter::convert_to_nm_keyboard_event(
		WinMessage win_message)
	{
		KeyboardEvent rst;
		switch(win_message.message){
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			rst.type = KEY_DOWN;
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			rst.type = KEY_UP;
			break;
		}

		rst.value = wink_to_key_info(
			static_cast<unsigned long>(win_message.w_param));

		return rst;
	}

	void STDWHIDInfoGetter::buffer_keyboard_event(UINT message,
		WPARAM wParam, LPARAM lParam)
	{
		WinMessage win_message;
		win_message.message = message;
		win_message.l_param = lParam;
		win_message.w_param = wParam;
		m_mutex.lock();
		bool may_need_notify = m_keyboard_message.empty();
		switch(message){
		case WM_KEYUP:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSKEYDOWN:
			m_keyboard_message.push(win_message);
			break;
		case WM_CHAR:
			break;
		}
		m_mutex.unlock();

		if(may_need_notify){
			m_promise_mutex.lock();
			m_promise.set_value(1);
			m_promise_mutex.unlock();
		}
	}

	void STDWHIDInfoGetter::buffer_mouse_event(UINT message,
		WPARAM wParam, LPARAM lParam)
	{
		m_mutex.lock();
		switch(message){
		case WM_MOUSEMOVE:
			break;
		case WM_MOUSEHWHEEL:
			break;
		case WM_MOUSEWHEEL:
			break;
		case WM_LBUTTONDBLCLK:
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			break;
		case WM_RBUTTONDBLCLK:
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_RBUTTONUP:
			break;
		case WM_MBUTTONDBLCLK:
			break;
		case WM_MBUTTONDOWN:
			break;
		case WM_MBUTTONUP:
			break;
		}
		m_mutex.unlock();
	}

	std::promise<int>& STDWHIDInfoGetter::set_promise(std::promise<int>&& promise){
		m_promise_mutex.lock();
		m_promise = std::move(promise);
		m_promise_mutex.unlock();
		return std::ref(m_promise);
	}

	KeyInfo STDWHIDInfoGetter::wink_to_key_info(unsigned long x){
		switch(x){
		case 0x30:  return NMK_0;
		case 0x31:  return NMK_1;
		case 0x32:  return NMK_2;
		case 0x33:  return NMK_3;
		case 0x34:  return NMK_4;
		case 0x35:  return NMK_5;
		case 0x36:  return NMK_6;
		case 0x37:  return NMK_7;
		case 0x38:  return NMK_8;
		case 0x39:  return NMK_9;
		case 0x60:  return NMK_NUMPAD0;
		case 0x61:  return NMK_NUMPAD1;
		case 0x62:  return NMK_NUMPAD2;
		case 0x63:  return NMK_NUMPAD3;
		case 0x64:  return NMK_NUMPAD4;
		case 0x65:  return NMK_NUMPAD5;
		case 0x66:  return NMK_NUMPAD6;
		case 0x67:  return NMK_NUMPAD7;
		case 0x68:  return NMK_NUMPAD8;
		case 0x69:  return NMK_NUMPAD9;
		case 0x41:  return NMK_A;
		case 0x42:  return NMK_B;
		case 0x43:  return NMK_C;
		case 0x44:  return NMK_D;
		case 0x45:  return NMK_E;
		case 0x46:  return NMK_F;
		case 0x47:  return NMK_G;
		case 0x48:  return NMK_H;
		case 0x49:  return NMK_I;
		case 0x4A:  return NMK_J;
		case 0x4B:  return NMK_K;
		case 0x4C:  return NMK_L;
		case 0x4D:  return NMK_M;
		case 0x4E:  return NMK_N;
		case 0x4F:  return NMK_O;
		case 0x50:  return NMK_P;
		case 0x51:  return NMK_Q;
		case 0x52:  return NMK_R;
		case 0x53:  return NMK_S;
		case 0x54:  return NMK_T;
		case 0x55:  return NMK_U;
		case 0x56:  return NMK_V;
		case 0x57:  return NMK_W;
		case 0x58:  return NMK_X;
		case 0x59:  return NMK_Y;
		case 0x5A:  return NMK_Z;
		case 0x70:  return NMK_F1;
		case 0x71:  return NMK_F2;
		case 0x72:  return NMK_F3;
		case 0x73:  return NMK_F4;
		case 0x74:  return NMK_F5;
		case 0x75:  return NMK_F6;
		case 0x76:  return NMK_F7;
		case 0x77:  return NMK_F8;
		case 0x78:  return NMK_F9;
		case 0x79:  return NMK_F10;
		case 0x7A:  return NMK_F11;
		case 0x7B:  return NMK_F12;
		case 0x7C:  return NMK_F13;
		case 0x7D:  return NMK_F14;
		case 0x7E:  return NMK_F15;
		case 0x08:  return NMK_BACK;
		case 0x09:  return NMK_TAB;
		case 0x20:  return NMK_SPACE;
		case 0x21:  return NMK_PRIOR;
		case 0x22:  return NMK_NEXT;
		case 0x23:  return NMK_END;
		case 0x24:  return NMK_HOME;
		case 0x25:  return NMK_LEFT;
		case 0x26:  return NMK_UP;
		case 0x27:  return NMK_RIGHT;
		case 0x28:  return NMK_DOWN;
		case 0x2D:  return NMK_INSERT;
		case 0x2E:  return NMK_DELETE;
		case 0x5B:  return NMK_LWIN;
		case 0x5C:  return NMK_RWIN;
		case 0x5D:  return NMK_APPS;
		case 0x5F:  return NMK_SLEEP;
		case 0x6A:  return NMK_MULTIPLY;
		case 0x6B:  return NMK_ADD;
		case 0x6D:  return NMK_SUBTRACT;
		case 0x6E:  return NMK_DECIMAL;
		case 0x6F:  return NMK_DIVIDE;
		case 0x90:  return NMK_NUMLOCK;
		case 0x91:  return NMK_SCROLL;
		case 0xA0:  return NMK_LSHIFT;
		case 0xA1:  return NMK_RSHIFT;
		case 0xA2:  return NMK_LCONTROL;
		case 0xA3:  return NMK_RCONTROL;
		case 0xA4:  return NMK_LMENU;
		case 0xA5:  return NMK_RMENU;
		case 0x0D:  return NMK_RETURN;
		case 0x13:  return NMK_PAUSE;
		case 0x14:  return NMK_CAPITAL;
		case 0x1B:  return NMK_ESCAPE;
		default: return NMK_INVALIDE;
		}
	}
}