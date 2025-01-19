#include "gfxengine/window.hpp"

#include "gfxengine/math.hpp"
#include "gfxengine/platform.hpp"
#include "gfxengine/graphics.hpp"
#include "gfxengine/window_event_handler.hpp"

extern std::unique_ptr<Graphics> _create_graphics();

#if GFXENGINE_PLATFORM_WINDOWS

#include "private/my_windows.hpp"
#include <glad/glad.h>
#include "private/wglext.h"

#pragma comment(lib, "opengl32.lib")

#include <vector>
#include <functional>
#include <optional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#if GFXENGINE_EDITOR
// TODO
#include "gfxengine/frame.hpp"

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#endif // GFXENGINE_EDITOR

class WindowsWindowStatic
{
private:

	WindowsWindowStatic()
	{
		hinstance = GetModuleHandleW(nullptr);

		{
			WNDCLASSW wc{};
			wc.style         = CS_OWNDC;
			wc.lpfnWndProc   = DefWindowProcW;
			wc.hInstance     = WindowsWindowStatic::hinstance;
			wc.hIcon         = LoadIconA(nullptr, IDI_WINLOGO);
			wc.hCursor       = LoadCursorA(nullptr, IDC_ARROW);
			wc.lpszClassName = class_name;

			if (!::RegisterClassW(&wc))
				throw 1;
				//ThrowSystemError("CWin32Window::RegisterClass(...): ");
		}

		for (auto &key : vkey_to_key)
			key = KeyboardEvent::Key::None;

		vkey_to_key[VK_SPACE] = KeyboardEvent::Key::Space;
		vkey_to_key[VK_OEM_7] = KeyboardEvent::Key::Apostrophe;
		vkey_to_key[VK_OEM_COMMA] = KeyboardEvent::Key::Comma;
		vkey_to_key[VK_OEM_MINUS] = KeyboardEvent::Key::Minus;
		vkey_to_key[VK_OEM_PERIOD] = KeyboardEvent::Key::Period;
		vkey_to_key[VK_OEM_2] = KeyboardEvent::Key::Slash;
		vkey_to_key[VK_OEM_1] = KeyboardEvent::Key::Semicolon;
		vkey_to_key[VK_OEM_PLUS] = KeyboardEvent::Key::Equal;

		vkey_to_key[VK_OEM_4] = KeyboardEvent::Key::LeftBracket;
		vkey_to_key[VK_OEM_5] = KeyboardEvent::Key::Backslash;
		vkey_to_key[VK_OEM_6] = KeyboardEvent::Key::RightBracket;
		vkey_to_key[VK_OEM_3] = KeyboardEvent::Key::GraveAccent;

		vkey_to_key[0x30] = KeyboardEvent::Key::K0;
		vkey_to_key[0x31] = KeyboardEvent::Key::K1;
		vkey_to_key[0x32] = KeyboardEvent::Key::K2;
		vkey_to_key[0x33] = KeyboardEvent::Key::K3;
		vkey_to_key[0x34] = KeyboardEvent::Key::K4;
		vkey_to_key[0x35] = KeyboardEvent::Key::K5;
		vkey_to_key[0x36] = KeyboardEvent::Key::K6;
		vkey_to_key[0x37] = KeyboardEvent::Key::K7;
		vkey_to_key[0x38] = KeyboardEvent::Key::K8;
		vkey_to_key[0x39] = KeyboardEvent::Key::K9;

		vkey_to_key[0x41] = KeyboardEvent::Key::A;
		vkey_to_key[0x42] = KeyboardEvent::Key::B;
		vkey_to_key[0x43] = KeyboardEvent::Key::C;
		vkey_to_key[0x44] = KeyboardEvent::Key::D;
		vkey_to_key[0x45] = KeyboardEvent::Key::E;
		vkey_to_key[0x46] = KeyboardEvent::Key::F;
		vkey_to_key[0x47] = KeyboardEvent::Key::G;
		vkey_to_key[0x48] = KeyboardEvent::Key::H;
		vkey_to_key[0x49] = KeyboardEvent::Key::I;
		vkey_to_key[0x4A] = KeyboardEvent::Key::J;
		vkey_to_key[0x4B] = KeyboardEvent::Key::K;
		vkey_to_key[0x4C] = KeyboardEvent::Key::L;
		vkey_to_key[0x4D] = KeyboardEvent::Key::M;
		vkey_to_key[0x4E] = KeyboardEvent::Key::N;
		vkey_to_key[0x4F] = KeyboardEvent::Key::O;
		vkey_to_key[0x50] = KeyboardEvent::Key::P;
		vkey_to_key[0x51] = KeyboardEvent::Key::Q;
		vkey_to_key[0x52] = KeyboardEvent::Key::R;
		vkey_to_key[0x53] = KeyboardEvent::Key::S;
		vkey_to_key[0x54] = KeyboardEvent::Key::T;
		vkey_to_key[0x55] = KeyboardEvent::Key::U;
		vkey_to_key[0x56] = KeyboardEvent::Key::V;
		vkey_to_key[0x57] = KeyboardEvent::Key::W;
		vkey_to_key[0x58] = KeyboardEvent::Key::X;
		vkey_to_key[0x59] = KeyboardEvent::Key::Y;
		vkey_to_key[0x5A] = KeyboardEvent::Key::Z;

		vkey_to_key[VK_ESCAPE] = KeyboardEvent::Key::Escape;
		//vkey_to_key[VK_RETURN] = KeyboardEvent::Key::Enter;
		vkey_to_key[VK_TAB] = KeyboardEvent::Key::Tab;
		vkey_to_key[VK_BACK] = KeyboardEvent::Key::Backspace;
		vkey_to_key[VK_INSERT] = KeyboardEvent::Key::Insert;
		vkey_to_key[VK_DELETE] = KeyboardEvent::Key::Delete;

		vkey_to_key[VK_LEFT] = KeyboardEvent::Key::Left;
		vkey_to_key[VK_UP] = KeyboardEvent::Key::Up;
		vkey_to_key[VK_RIGHT] = KeyboardEvent::Key::Right;
		vkey_to_key[VK_DOWN] = KeyboardEvent::Key::Down;

		vkey_to_key[VK_PRIOR] = KeyboardEvent::Key::PageUp;
		vkey_to_key[VK_NEXT] = KeyboardEvent::Key::PageDown;
		vkey_to_key[VK_HOME] = KeyboardEvent::Key::Home;
		vkey_to_key[VK_END] = KeyboardEvent::Key::End;

		vkey_to_key[VK_CAPITAL] = KeyboardEvent::Key::CapsLock;
		vkey_to_key[VK_SCROLL] = KeyboardEvent::Key::ScrollLock;
		vkey_to_key[VK_NUMLOCK] = KeyboardEvent::Key::NumLock;
		vkey_to_key[VK_SNAPSHOT] = KeyboardEvent::Key::PrintScreen;
		vkey_to_key[VK_PAUSE] = KeyboardEvent::Key::Pause;
	
		vkey_to_key[VK_F1] = KeyboardEvent::Key::F1;
		vkey_to_key[VK_F2] = KeyboardEvent::Key::F2;
		vkey_to_key[VK_F3] = KeyboardEvent::Key::F3;
		vkey_to_key[VK_F4] = KeyboardEvent::Key::F4;
		vkey_to_key[VK_F5] = KeyboardEvent::Key::F5;
		vkey_to_key[VK_F6] = KeyboardEvent::Key::F6;
		vkey_to_key[VK_F7] = KeyboardEvent::Key::F7;
		vkey_to_key[VK_F8] = KeyboardEvent::Key::F8;
		vkey_to_key[VK_F9] = KeyboardEvent::Key::F9;
		vkey_to_key[VK_F10] = KeyboardEvent::Key::F10;
		vkey_to_key[VK_F11] = KeyboardEvent::Key::F11;
		vkey_to_key[VK_F12] = KeyboardEvent::Key::F12;
		vkey_to_key[VK_F13] = KeyboardEvent::Key::F13;
		vkey_to_key[VK_F14] = KeyboardEvent::Key::F14;
		vkey_to_key[VK_F15] = KeyboardEvent::Key::F15;
		vkey_to_key[VK_F16] = KeyboardEvent::Key::F16;
		vkey_to_key[VK_F17] = KeyboardEvent::Key::F17;
		vkey_to_key[VK_F18] = KeyboardEvent::Key::F18;
		vkey_to_key[VK_F19] = KeyboardEvent::Key::F19;
		vkey_to_key[VK_F20] = KeyboardEvent::Key::F20;
		vkey_to_key[VK_F21] = KeyboardEvent::Key::F21;
		vkey_to_key[VK_F22] = KeyboardEvent::Key::F22;
		vkey_to_key[VK_F23] = KeyboardEvent::Key::F23;
		vkey_to_key[VK_F24] = KeyboardEvent::Key::F24;

		vkey_to_key[VK_NUMPAD0] = KeyboardEvent::Key::Kp0;
		vkey_to_key[VK_NUMPAD1] = KeyboardEvent::Key::Kp1;
		vkey_to_key[VK_NUMPAD2] = KeyboardEvent::Key::Kp2;
		vkey_to_key[VK_NUMPAD3] = KeyboardEvent::Key::Kp3;
		vkey_to_key[VK_NUMPAD4] = KeyboardEvent::Key::Kp4;
		vkey_to_key[VK_NUMPAD5] = KeyboardEvent::Key::Kp5;
		vkey_to_key[VK_NUMPAD6] = KeyboardEvent::Key::Kp6;
		vkey_to_key[VK_NUMPAD7] = KeyboardEvent::Key::Kp7;
		vkey_to_key[VK_NUMPAD8] = KeyboardEvent::Key::Kp8;
		vkey_to_key[VK_NUMPAD9] = KeyboardEvent::Key::Kp9;

		vkey_to_key[VK_DECIMAL] = KeyboardEvent::Key::KpDecimal;
		vkey_to_key[VK_DIVIDE] = KeyboardEvent::Key::KpDivide;
		vkey_to_key[VK_MULTIPLY] = KeyboardEvent::Key::KpMultiply;
		vkey_to_key[VK_SUBTRACT] = KeyboardEvent::Key::KpSubtract;
		vkey_to_key[VK_ADD] = KeyboardEvent::Key::KpAdd;
		//vkey_to_key[VK_RETURN] = KeyboardEvent::Key::KpEnter;
		//KpEqual

		//vkey_to_key[VK_LSHIFT] = KeyboardEvent::Key::LeftShift;
		//vkey_to_key[VK_LCONTROL] = KeyboardEvent::Key::LeftControl;
		//vkey_to_key[VK_LMENU] = KeyboardEvent::Key::LeftAlt;
		//vkey_to_key[VK_LWIN] = KeyboardEvent::Key::LeftSuper;

		//vkey_to_key[VK_RSHIFT] = KeyboardEvent::Key::RightShift;
		//vkey_to_key[VK_RCONTROL] = KeyboardEvent::Key::RightControl;
		//vkey_to_key[VK_RMENU] = KeyboardEvent::Key::RightAlt;
		//vkey_to_key[VK_RWIN] = KeyboardEvent::Key::RightSuper;

		vkey_to_key[VK_APPS] = KeyboardEvent::Key::Menu;
	}

	~WindowsWindowStatic()
	{
	}

	WindowsWindowStatic(WindowsWindowStatic const &) = delete;
	WindowsWindowStatic &operator = (WindowsWindowStatic const &) = delete;
	WindowsWindowStatic(WindowsWindowStatic &&) = delete;
	WindowsWindowStatic &operator = (WindowsWindowStatic &&) = delete;

public:

	static inline HINSTANCE hinstance;
	static inline KeyboardEvent::Key vkey_to_key[256];

	static inline wchar_t const *class_name = L"WindowsWindowClass";

	static WindowsWindowStatic &get_singleton()
	{
		static WindowsWindowStatic instance;
		return instance;
	}
};

class WindowsWindow : public Window
{
private:

	Platform &platform;

	HWND hwnd = nullptr;
	HDC hdc = nullptr;
	HGLRC hglrc = nullptr;

	bool closed = false;
	bool raw_mouse = false;
	POINT save_mouse_pos{};

	ivec2 viewport_size{};

	bool fullscreen_enabled = false;
	RECT save_window_rect{};
	WINDOWPLACEMENT save_window_placement{ sizeof(WINDOWPLACEMENT) };

	std::unique_ptr<Graphics> graphics;

	WindowEventHandler *window_event_handler = nullptr;
	
#if GFXENGINE_EDITOR
	bool imgui_initialized = false;
#endif // GFXENGINE_EDITOR

	LRESULT win_proc(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		// printf("m: 0x%08x, w: 0x%08x, l: 0x%08x\n", UINT(msg), UINT(wparam), UINT(lparam));

#if GFXENGINE_EDITOR
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
			return true;
#endif

		switch (msg)
		{
			case WM_LBUTTONDOWN:
			{
				double event_time = platform.get_time();
				MouseEvent event{};
				event.type = MouseEvent::Type::Press;
				event.key = MouseEvent::Key::LMB;
				event.locked = raw_mouse;
				if (!raw_mouse)
					event.pos = { LOWORD(lparam), HIWORD(lparam) };
				window_event_handler->on_mouse_event(event_time, event);
				break;
			}
			case WM_LBUTTONUP:
			{
				double event_time = platform.get_time();
				MouseEvent event{};
				event.type = MouseEvent::Type::Release;
				event.key = MouseEvent::Key::LMB;
				event.locked = raw_mouse;
				if (!raw_mouse)
					event.pos = { LOWORD(lparam), HIWORD(lparam) };
				window_event_handler->on_mouse_event(event_time, event);
				break;
			}
			case WM_MBUTTONDOWN:
			{
				double event_time = platform.get_time();
				MouseEvent event{};
				event.type = MouseEvent::Type::Press;
				event.key = MouseEvent::Key::MMB;
				event.locked = raw_mouse;
				if (!raw_mouse)
					event.pos = { LOWORD(lparam), HIWORD(lparam) };
				window_event_handler->on_mouse_event(event_time, event);
				break;
			}
			case WM_MBUTTONUP:
			{
				double event_time = platform.get_time();
				MouseEvent event{};
				event.type = MouseEvent::Type::Release;
				event.key = MouseEvent::Key::MMB;
				event.locked = raw_mouse;
				if (!raw_mouse)
					event.pos = { LOWORD(lparam), HIWORD(lparam) };
				window_event_handler->on_mouse_event(event_time, event);
				break;
			}
			case WM_RBUTTONDOWN:
			{
				double event_time = platform.get_time();
				MouseEvent event{};
				event.type = MouseEvent::Type::Press;
				event.key = MouseEvent::Key::RMB;
				event.locked = raw_mouse;
				if (!raw_mouse)
					event.pos = { LOWORD(lparam), HIWORD(lparam) };
				window_event_handler->on_mouse_event(event_time, event);
				break;
			}
			case WM_RBUTTONUP:
			{
				double event_time = platform.get_time();
				MouseEvent event{};
				event.type = MouseEvent::Type::Release;
				event.key = MouseEvent::Key::RMB;
				event.locked = raw_mouse;
				if (!raw_mouse)
					event.pos = { LOWORD(lparam), HIWORD(lparam) };
				window_event_handler->on_mouse_event(event_time, event);
				break;
			}
			case WM_MOUSEMOVE:
			{
				if (raw_mouse)
					break;

				double event_time = platform.get_time();
				MouseEvent event{};
				event.type = MouseEvent::Type::Move;
				event.locked = raw_mouse;
				if (!raw_mouse)
					event.pos = { LOWORD(lparam), HIWORD(lparam) };
				window_event_handler->on_mouse_event(event_time, event);
				break;
			}
			case WM_MOUSEHWHEEL:
			{
				double event_time = platform.get_time();
				MouseEvent event{};
				event.type = MouseEvent::Type::Scroll;
				event.locked = raw_mouse;
				if (!raw_mouse)
					event.pos = { LOWORD(lparam), HIWORD(lparam) };
				event.scroll.x = (float)GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA;
				window_event_handler->on_mouse_event(event_time, event);
				break;
			}
			case WM_MOUSEWHEEL:
			{
				double event_time = platform.get_time();
				MouseEvent event{};
				event.type = MouseEvent::Type::Scroll;
				event.locked = raw_mouse;
				if (!raw_mouse)
					event.pos = { LOWORD(lparam), HIWORD(lparam) };
				event.scroll.y = (float)GET_WHEEL_DELTA_WPARAM(wparam) / (float)WHEEL_DELTA;
				window_event_handler->on_mouse_event(event_time, event);
				break;
			}

			case WM_INPUT:
			{
				if (!raw_mouse)
					break;

				double event_time = platform.get_time();

				RAWINPUT raw;
				UINT raw_size = sizeof(raw);

				GetRawInputData((HRAWINPUT)lparam, RID_INPUT, &raw, &raw_size, sizeof(RAWINPUTHEADER));

				if (raw.header.dwType == RIM_TYPEMOUSE) 
				{
					int xPosRelative = raw.data.mouse.lLastX;
					int yPosRelative = raw.data.mouse.lLastY;

					MouseEvent event{};
					event.type = MouseEvent::Type::Move;
					event.locked = raw_mouse;
					event.pos = { raw.data.mouse.lLastX, raw.data.mouse.lLastY };
					window_event_handler->on_mouse_event(event_time, event);
				}

				break;
			}

			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				double event_time = platform.get_time();

				KeyboardEvent event{};

				if ((lparam & 0x40000000) == 0)
					event.type = KeyboardEvent::Type::Press;
				else
					event.type = KeyboardEvent::Type::Repeat;

				if (wparam < (sizeof(WindowsWindowStatic::vkey_to_key) / sizeof(WindowsWindowStatic::vkey_to_key[0])))
					event.key = WindowsWindowStatic::vkey_to_key[wparam];

				if (event.key == KeyboardEvent::Key::None)
				{
					switch (wparam)
					{
						case VK_SHIFT:
							// RightShift Undocumented?
							event.key = (lparam & 0x00100000) ? KeyboardEvent::Key::RightShift : KeyboardEvent::Key::LeftShift;
							break;
						case VK_CONTROL:
							event.key = (lparam & 0x01000000) ? KeyboardEvent::Key::RightControl : KeyboardEvent::Key::LeftControl;
							break;
						case VK_MENU:
							event.key = (lparam & 0x01000000) ? KeyboardEvent::Key::RightAlt : KeyboardEvent::Key::LeftAlt;
							break;
						case VK_RETURN:
							event.key = (lparam & 0x01000000) ? KeyboardEvent::Key::KpEnter : KeyboardEvent::Key::Enter;
							break;
					}
				}

				if (event.key == KeyboardEvent::Key::None)
					break;

				event.locked = raw_mouse;

				window_event_handler->on_keyboard_event(event_time, event);
				break;
			}
			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				double event_time = platform.get_time();

				KeyboardEvent event{};

				event.type = KeyboardEvent::Type::Release;

				if (wparam < (sizeof(WindowsWindowStatic::vkey_to_key) / sizeof(WindowsWindowStatic::vkey_to_key[0])))
					event.key = WindowsWindowStatic::vkey_to_key[wparam];

				if (event.key == KeyboardEvent::Key::None)
				{
					switch (wparam)
					{
						case VK_SHIFT:
							event.key = (lparam & 0x00100000) ? KeyboardEvent::Key::RightShift : KeyboardEvent::Key::LeftShift;
							break;
						case VK_CONTROL:
							event.key = (lparam & 0x01000000) ? KeyboardEvent::Key::RightControl : KeyboardEvent::Key::LeftControl;
							break;
						case VK_MENU:
							event.key = (lparam & 0x01000000) ? KeyboardEvent::Key::RightAlt : KeyboardEvent::Key::LeftAlt;
							break;
						case VK_RETURN:
							event.key = (lparam & 0x01000000) ? KeyboardEvent::Key::Enter : KeyboardEvent::Key::KpEnter;
							break;
					}
				}

				if (event.key == KeyboardEvent::Key::None)
					break;

				event.locked = raw_mouse;

				window_event_handler->on_keyboard_event(event_time, event);
				break;
			}

			case WM_SIZE:
			{
				double event_time = platform.get_time();
				ResizeEvent event{};
				event.old_size = viewport_size;
				event.new_size = { LOWORD(lparam), HIWORD(lparam) };
				viewport_size = event.new_size;
				window_event_handler->on_resize(event_time, event);
				break;
			}

			case WM_CAPTURECHANGED:
			{
				if (raw_mouse)
				{
					lock_mouse(false);
					double event_time = platform.get_time();
					MouseExternalUnlockEvent event{};
					window_event_handler->on_mouse_external_unlock(event_time, event);
				}
				break;
			}

			case WM_SYSCOMMAND:
			{
				// block system commands (like opening the context menu with Alt+Space)
				if (wparam == SC_KEYMENU)
					return 0;

				break;
			}

			case WM_CLOSE:
			{
				double event_time = platform.get_time();
				CloseEvent event{};
				window_event_handler->on_close_event(event_time, event);
				closed = true;
				break;
			}
		}

		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}

	static LRESULT CALLBACK static_win_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		WindowsWindow *self = (WindowsWindow *)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
		return self->win_proc(msg, wparam, lparam);
	}

	static HGLRC create_gl_context(HDC hdc, int version_major, int version_minor) noexcept
	{
		HGLRC hglrc = wglCreateContext(hdc);

		if (!hglrc)
			return 0;

		if (!wglMakeCurrent(hdc, hglrc))
		{
			wglDeleteContext(hglrc);
			return 0;
		}

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));

		if (!wglCreateContextAttribsARB)
			return hglrc;

		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(hglrc);

		int gl_attribs[]{
			WGL_CONTEXT_MAJOR_VERSION_ARB, version_major,
			WGL_CONTEXT_MINOR_VERSION_ARB, version_minor,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0, 0,
		};

		return wglCreateContextAttribsARB(hdc, 0, gl_attribs);
	}

public:

	WindowsWindow(CreateWindowParams const &params)
		: platform{ params.platform }
	{
		(void)WindowsWindowStatic::get_singleton();

		auto WindowStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		hwnd = CreateWindowExW(0, WindowsWindowStatic::class_name, L"LearnOpenGL", WindowStyle, 200, 200, 1000, 1000, nullptr, nullptr, WindowsWindowStatic::hinstance, nullptr);

		if (!hwnd)
			throw 1;
			//ThrowSystemError("CWin32Window::CreateWindow(...): ");

		if (params.window_event_handler)
		{
			window_event_handler = params.window_event_handler;

			SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
			SetWindowLongPtrW(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowsWindow::static_win_proc));
			//SetWindowsHookExW()
		}

		{
			LONG_PTR lExStyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
			lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);

			// May return ZERO if previous data was ZERO
			if (!SetWindowLongPtrW(hwnd, GWL_EXSTYLE, lExStyle))
			{
				auto ErrCode = GetLastError();

				if (ErrCode)
					throw 1;
					//ThrowSystemError(ErrCode, "CWin32Window::SetWindowLongPtr(GWL_EXSTYLE): ");
			}
		}

		hdc = GetDC(hwnd);

		{
			PIXELFORMATDESCRIPTOR pfd{};

			pfd.nSize        = sizeof(pfd);
			pfd.nVersion     = 1;
			pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType   = PFD_TYPE_RGBA;
			pfd.cColorBits   = 32;

			int pf = ChoosePixelFormat(hdc, &pfd);

			if (!pf)
				throw 1;
				//ThrowSystemError("CWin32Window::ChoosePixelFormat(...): ");

			if (SetPixelFormat(hdc, pf, &pfd) == FALSE)
				throw 1;
				//ThrowSystemError("CWin32Window::SetPixelFormat(...): ");

			if (!DescribePixelFormat(hdc, pf, sizeof(pfd), &pfd))
				throw 1;
				//ThrowSystemError("CWin32Window::DescribePixelFormat(...): ");
		}

		ShowWindow(hwnd, SW_SHOW);
	}

	~WindowsWindow()
	{
		if (hwnd)
		{
#if GFXENGINE_EDITOR
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
#endif // GFXENGINE_EDITOR

			graphics.reset();
			//wglDeleteContext(hglrc);
			DestroyWindow(hwnd);
		}
	}

	virtual void poll_events() override
	{
		MSG msg;

		while (PeekMessageW(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	virtual void draw(Frame const &frame) override
	{
#if GFXENGINE_EDITOR
		if (!imgui_initialized)
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO &io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// Setup Platform/Renderer backends
			ImGui_ImplWin32_InitForOpenGL(hwnd);
			ImGui_ImplOpenGL3_Init();

			ImGui_ImplOpenGL3_NewFrame();

			imgui_initialized = true;
		}

		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (frame.draw_editor)
			frame.draw_editor();

		ImGui::Render();
#endif // GFXENGINE_EDITOR

		graphics->draw(frame);

#if GFXENGINE_EDITOR
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif // GFXENGINE_EDITOR

		SwapBuffers(hdc);
	}

	virtual Graphics &get_graphics() const override
	{
		return *graphics;
	}

	virtual void lock_mouse(bool lock) override
	{
		if (lock)
		{
			GetCursorPos(&save_mouse_pos);

#if GFXENGINE_EDITOR
			ImGuiIO &io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
#endif // GFXENGINE_EDITOR

			ShowCursor(FALSE);

			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			ClientToScreen(hwnd, (LPPOINT)&clientRect.left);
			ClientToScreen(hwnd, (LPPOINT)&clientRect.right);
			ClipCursor(&clientRect);

			SetCursorPos((clientRect.left + clientRect.right) / 2, (clientRect.top + clientRect.bottom) / 2);

			const RAWINPUTDEVICE rid = { 0x01, 0x02, 0, hwnd };

			RegisterRawInputDevices(&rid, 1, sizeof(rid));

			SetCapture(hwnd);

			raw_mouse = true;
		}
		else
		{
			raw_mouse = false;

			ReleaseCapture();

			ClipCursor(nullptr);

#if GFXENGINE_EDITOR
			ImGuiIO &io = ImGui::GetIO(); (void)io;
			io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
#endif // GFXENGINE_EDITOR

			const RAWINPUTDEVICE rid = { 0x01, 0x02, RIDEV_REMOVE, NULL };

			RegisterRawInputDevices(&rid, 1, sizeof(rid));

			SetCursorPos(save_mouse_pos.x, save_mouse_pos.y);

			ShowCursor(TRUE);
		}
	}

	virtual void fullscreen(bool enable) override
	{
		fullscreen_enabled = enable;
		DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

		if (enable)
		{
			MONITORINFO mi{ sizeof(mi) };

			if (GetWindowPlacement(hwnd, &save_window_placement) && GetMonitorInfoW(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mi))
			{
				SetWindowLongW(hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(hwnd, HWND_TOP,
							mi.rcMonitor.left, mi.rcMonitor.top,
							mi.rcMonitor.right - mi.rcMonitor.left,
							mi.rcMonitor.bottom - mi.rcMonitor.top,
							SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
		}
		else
		{
			SetWindowLongW(hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
			SetWindowPlacement(hwnd, &save_window_placement);
			SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
							SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
							SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}

	virtual void close() override
	{
		PostMessageW(hwnd, WM_CLOSE, 0, 0);
	}

	virtual void set_vsync(bool enable) override
	{
		// TODO
		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
		wglSwapIntervalEXT(enable ? 1 : 0);
	}

	void init_graphics()
	{
		hglrc = create_gl_context(hdc, 4, 6);

		if (!hglrc)
			throw 1;
			//ThrowSystemError("CWin32Window::wglCreateContext(...): ");

		wglMakeCurrent(hdc, hglrc);

		graphics = _create_graphics();
	}

	bool wait_events()
	{
		if (closed)
			return true;

		MSG msg;

		if (BOOL ret = GetMessageW(&msg, hwnd, 0, 0); ret != 0)
		{
			if (ret < 0)
			{
				auto a = GetLastError();
				throw 1;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		while (PeekMessageW(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return false;
	}

	HWND get_hwnd() const
	{
		return hwnd;
	}
};

using WindowEventVariant = std::variant<
	KeyboardEvent,
	MouseEvent,
	MouseExternalUnlockEvent,
	ResizeEvent,
	CloseEvent
>;

struct WindowEventWrapper
{
	double time;
	WindowEventVariant data;
};

class WindowEventHandlerThread : public WindowEventHandler
{
private:

	std::mutex mtx;
	std::vector<WindowEventWrapper> events;
	std::vector<WindowEventWrapper> events2;

public:

	WindowEventHandlerThread()
	{
		events.reserve(16);
		events2.reserve(16);
	}

	virtual void on_keyboard_event(double time, KeyboardEvent event) override
	{
		std::unique_lock lck(mtx);
		events.push_back(WindowEventWrapper(time, event));
	}

	virtual void on_mouse_event(double time, MouseEvent event) override
	{
		std::unique_lock lck(mtx);
		events.push_back(WindowEventWrapper(time, event));
	}

	virtual void on_mouse_external_unlock(double time, MouseExternalUnlockEvent event) override
	{
		std::unique_lock lck(mtx);
		events.push_back(WindowEventWrapper(time, event));
	}

	virtual void on_resize(double time, ResizeEvent event) override
	{
		std::unique_lock lck(mtx);
		events.push_back(WindowEventWrapper(time, event));
	}

	virtual void on_close_event(double time, CloseEvent event) override
	{
		std::unique_lock lck(mtx);
		events.push_back(WindowEventWrapper(time, event));
	}

	[[nodiscard]]
	std::vector<WindowEventWrapper> const &get_and_release_events()
	{
		std::unique_lock lck(mtx);
		events2.clear();
		events.swap(events2);
		return events2;
	}
};

class WindowsWindowThread : public Window
{
private:

	std::thread window_thread;
	WindowEventHandlerThread event_handler;
	WindowEventHandler *window_event_handler = nullptr;

	std::mutex mtx;
	bool cv_notified = false;
	std::condition_variable cv;
	bool cv_destroy_notified = false;
	std::condition_variable cv_destroy;
	std::vector<std::function<void(WindowsWindow &window)>> tasks;
	std::vector<std::function<void(WindowsWindow &window)>> tasks2;

	WindowsWindow *_window = nullptr;

	WindowsWindowThread(WindowsWindowThread const &) = delete;
	WindowsWindowThread &operator = (WindowsWindowThread const &) = delete;
	WindowsWindowThread(WindowsWindowThread &&) = delete;
	WindowsWindowThread &operator = (WindowsWindowThread &&) = delete;

	auto synchronized(auto const &func)
	{
		std::unique_lock lck(mtx);
		return func(lck);
	}

	void cv_wait(std::condition_variable &_cv, bool &_cv_notified)
	{
		synchronized([&](auto &lck) {
			_cv.wait(lck, [&](){ return _cv_notified; });
			_cv_notified = false;
		});
	}

	void cv_notify(std::condition_variable &_cv, bool &_cv_notified)
	{
		synchronized([&](auto &lck) {
			_cv_notified = true;
		});
		_cv.notify_one();
	}

	void push_task(std::function<void(WindowsWindow &window)> task)
	{
		synchronized([&](auto &lck) {
			tasks.push_back(std::move(task));
		});

		// Wake up window thread
		PostMessageW(_window->get_hwnd(), WM_USER, 0, 0);
	}

	template <typename TRet>
	TRet push_task_and_wait(std::function<TRet(WindowsWindow &window)> task)
	{
		if constexpr (std::is_same_v<void, TRet>)
		{
			push_task([&](WindowsWindow &window) {
				task(window);
				cv_notify(cv, cv_notified);
			});

			cv_wait(cv, cv_notified);
		}
		else
		{
			std::optional<TRet> ret;

			push_task([&](WindowsWindow &window) {
				ret = task(window);
				cv_notify(cv, cv_notified);
			});

			cv_wait(cv, cv_notified);

			return ret.value();
		}
	}

	[[nodiscard]]
	std::vector<std::function<void(WindowsWindow &window)>> const &get_and_release_tasks()
	{
		synchronized([&](auto &lck) {
			tasks2.clear();
			tasks.swap(tasks2);
		});
		return tasks2;
	}

public:

	WindowsWindowThread(CreateWindowParams const &params)
	{
		tasks.reserve(16);
		tasks2.reserve(16);

		CreateWindowParams copy_params = params;

		if (params.window_event_handler)
		{
			window_event_handler = copy_params.window_event_handler;
			copy_params.window_event_handler = &event_handler;
		}

		window_thread = std::thread([this, copy_params]() {
			WindowsWindow window(copy_params);
			_window = &window;

			cv_notify(cv, cv_notified);

			while (true)
			{
				bool stop = window.wait_events();

				auto const &ts = get_and_release_tasks();

				for (auto const &t : ts)
					t(window);

				if (stop)
					break;
			}

			cv_wait(cv_destroy, cv_destroy_notified);

			_window = nullptr;
		});

		cv_wait(cv, cv_notified);

		_window->init_graphics();
	}

	~WindowsWindowThread()
	{
		close();
		cv_notify(cv_destroy, cv_destroy_notified);
		window_thread.join();
	}

	virtual void poll_events() override
	{
		auto const &events = event_handler.get_and_release_events();

		for (auto const &event : events)
		{
			std::visit([&](auto &data) {
				using T = std::decay_t<decltype(data)>;

				if constexpr (std::is_same_v<T, KeyboardEvent>)
				{
					window_event_handler->on_keyboard_event(event.time, data);
				}
				else
				if constexpr (std::is_same_v<T, MouseEvent>)
				{
					window_event_handler->on_mouse_event(event.time, data);
				}
				else
				if constexpr (std::is_same_v<T, MouseExternalUnlockEvent>)
				{
					window_event_handler->on_mouse_external_unlock(event.time, data);
				}
				else
				if constexpr (std::is_same_v<T, ResizeEvent>)
				{
					window_event_handler->on_resize(event.time, data);
				}
				else
				if constexpr (std::is_same_v<T, CloseEvent>)
				{
					window_event_handler->on_close_event(event.time, data);
				}
			}, event.data);
		}
	}

	virtual void draw(Frame const &frame) override
	{
		_window->draw(frame);
	}

	virtual Graphics &get_graphics() const override
	{
		return _window->get_graphics();
	}

	virtual void lock_mouse(bool lock) override
	{
		push_task([lock](WindowsWindow &window) {
			window.lock_mouse(lock);
		});
	}

	virtual void fullscreen(bool enable) override
	{
		if (_window)
			_window->fullscreen(enable);
	}

	virtual void close() override
	{
		push_task([](WindowsWindow &window) {
			window.close();
		});
	}

	virtual void set_vsync(bool enable) override
	{
		if (_window)
			_window->set_vsync(enable);
	}
};

std::unique_ptr<Window> _create_window(CreateWindowParams const &params)
{
	return std::make_unique<WindowsWindowThread>(params);
}

#else // GFXENGINE_PLATFORM_WINDOWS

#error Unknown platform

#endif
