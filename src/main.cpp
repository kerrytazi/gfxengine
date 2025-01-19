#include "gfxengine/platform.hpp"

extern std::unique_ptr<Platform> _create_platform();
extern void run_app(Platform &platform);

void startup()
{
	auto platform = _create_platform();
	run_app(*platform);
}

int WinMain()
{
	startup();
	return 0;
}

int main()
{
	startup();
	return 0;
}
