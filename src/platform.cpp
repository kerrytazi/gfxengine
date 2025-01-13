#include "gfxengine/platform.hpp"

#include "gfxengine/window.hpp"

#include "private/my_windows.hpp"

extern std::unique_ptr<Window> _create_window(CreateWindowParams const &params);

using NTSTATUS = LONG;
static NTSTATUS(__stdcall *NtDelayExecution)(BOOL Alertable, PLARGE_INTEGER DelayInterval) = (NTSTATUS(__stdcall*)(BOOL, PLARGE_INTEGER)) GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtDelayExecution");
static NTSTATUS(__stdcall *ZwSetTimerResolution)(IN ULONG RequestedResolution, IN BOOLEAN Set, OUT PULONG ActualResolution) = (NTSTATUS(__stdcall*)(ULONG, BOOLEAN, PULONG)) GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "ZwSetTimerResolution");

class WindowsPlatform : public Platform
{
private:

	double frequency = 0;

public:

	WindowsPlatform()
	{
		{
			LARGE_INTEGER value;
			QueryPerformanceFrequency(&value);
			frequency = (double)value.QuadPart;
		}

		{
			ULONG actualResolution;
			ZwSetTimerResolution(1, true, &actualResolution);
		}
	}

	virtual std::unique_ptr<Window> create_window(CreateWindowParams const &params) const override
	{
		return _create_window(params);
	}

	virtual uint64_t get_system_time_ms() const override
	{
		uint64_t t = 0;
		GetSystemTimeAsFileTime((FILETIME *)&t);
		return t / 10000 - 0x19DB1DED53E8000ull / 10000;
	}

	virtual double get_time() const override
	{
		uint64_t value;
		QueryPerformanceCounter((LARGE_INTEGER *)&value);
		return (double)value / frequency;
	}

	virtual void sleep(double seconds) override
	{
		LARGE_INTEGER interval;
		interval.QuadPart = seconds * -10000000.0;
		NtDelayExecution(false, &interval);
	}

	virtual void debug_output(char const *c_str, size_t len) override
	{
		OutputDebugStringA(c_str);
	}
};

std::unique_ptr<Platform> _create_platform()
{
	return std::make_unique<WindowsPlatform>();
}
