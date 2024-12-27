#include "gfxengine/base_application.hpp"
#include "gfxengine/platform.hpp"

extern std::unique_ptr<BasePlatform> _create_platform();

void startup()
{
	auto platform = _create_platform();
	auto app = create_application(*platform);
	app->start();
}

#if GFXENGINE_PLATFORM_WINDOWS
int WinMain()
{
	startup();
	return 0;
}
#else
int main()
{
	startup();
	return 0;
}
#endif // GFXENGINE_PLATFORM_WINDOWS
