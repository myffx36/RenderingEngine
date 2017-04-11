#include "stdafx.h"
#include "D3D11_1PIL.h"

#include "STDWHIDInfoGetter.h"
#include "D3D11_1LLGI.h"
#include "D3D11ShaderManager.h"

namespace Necromancer{
	namespace{
		STDWHIDInfoGetter* stdw_hid_info_getter;
		LRESULT CALLBACK wnd_proc( HWND hWnd, UINT message, 
			WPARAM wParam, LPARAM lParam )
		{
			PAINTSTRUCT ps;
			HDC hdc;

			switch( message )
			{
			case WM_PAINT:
				hdc = BeginPaint( hWnd, &ps );
				EndPaint( hWnd, &ps );
				break;

			case WM_DESTROY:
				PostQuitMessage( 0 );
				break;
			case WM_KEYUP:
			case WM_KEYDOWN:
			case WM_CHAR:
			case WM_SYSKEYUP:
			case WM_SYSKEYDOWN:
				stdw_hid_info_getter->buffer_keyboard_event(message, wParam, lParam);
				break;

			case WM_MOUSEMOVE:
			case WM_MOUSEHWHEEL:
			case WM_MOUSEWHEEL:
			case WM_LBUTTONDBLCLK:
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDBLCLK:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDBLCLK:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
				stdw_hid_info_getter->buffer_mouse_event(message, wParam, lParam);
				break;

			default:
				return DefWindowProc( hWnd, message, wParam, lParam );
				break;
			}

			return 0;
		}
	}
	D3D11_1PIL::D3D11_1PIL(Configuration* config)
		:PIL(config)
	{
		m_quit = false;
		m_stdw_hid_info_getter = new STDWHIDInfoGetter();
		stdw_hid_info_getter = m_stdw_hid_info_getter;

		m_thread = std::move(std::thread(
			&::Necromancer::D3D11_1PIL::thread_main, this, config));
		
		m_init_finish_flag.get_future().get();
	}

	D3D11_1PIL::~D3D11_1PIL(){
		delete pil_hid_info_getter();
		delete pil_shader_manager();
		delete pil_llgi();
	}

	void D3D11_1PIL::thread_main(Configuration* config){
		create_window(config);

		D3D11_1LLGI* d3d11_1_llgi = new D3D11_1LLGI(config, m_hwnd);
		set_pil_llgi(d3d11_1_llgi);
		set_pil_shader_manager(new D3D11ShaderManager());
		set_pil_hid_info_getter(m_stdw_hid_info_getter);

		set_screen_width(config->get_integer_value("Screen", "width"));
		set_screen_height(config->get_integer_value("Screen", "height"));

		m_init_finish_flag.set_value(1);

		MSG msg = {0};

		while (GetMessage(&msg, nullptr, 0, 0) && !m_quit)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void D3D11_1PIL::send_end_message(){
		m_quit = true;
	}

	void D3D11_1PIL::wait_for_end(){
		m_thread.join();
	}

	void D3D11_1PIL::create_window(Configuration* config){
		unsigned long width = config->get_integer_value("Screen", "width");
		unsigned long height = config->get_integer_value("Screen", "height");

		WNDCLASSEX wcex;
		wcex.cbSize = sizeof( WNDCLASSEX );
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = wnd_proc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandle(nullptr);
		wcex.hIcon = nullptr;
		wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
		wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = L"NecWindow";
		wcex.hIconSm = nullptr;
		if( !RegisterClassEx( &wcex ) )
			return;

		RECT rc = { 0, 0, width, height };
		AdjustWindowRect( &rc, WS_POPUP, FALSE );
		m_hwnd = CreateWindow( L"NecWindow", L"NecWindow",
			WS_POPUP,
			CW_USEDEFAULT, CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top,
			NULL, NULL, GetModuleHandle(nullptr), NULL );
		if(!m_hwnd) return;

		ShowWindow(m_hwnd, 1);
		UpdateWindow(m_hwnd);
	}
}