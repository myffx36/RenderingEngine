#pragma once

#include "PIL.h"

namespace Necromancer{
	class STDWHIDInfoGetter;
	class D3D11_1PIL:public PIL{
	public:
		D3D11_1PIL(Configuration*);

		virtual ~D3D11_1PIL();

		static LRESULT CALLBACK info_getter_wnd_proc( 
			HWND hWnd, UINT message, 
			WPARAM wParam, LPARAM lParam );

		virtual void wait_for_end();
		virtual void send_end_message();
	private:
		virtual void thread_main(Configuration*);
		virtual void create_window(Configuration*);
		/*virtual void init_others(Configuration*);
		virtual void main_loop();*/
	private:
		STDWHIDInfoGetter* m_stdw_hid_info_getter;
		HWND m_hwnd;
		std::thread m_thread;
		std::promise<int> m_init_finish_flag;
		bool m_quit;
	};
}

