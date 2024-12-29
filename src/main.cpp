#include "gfxengine/base_application.hpp"
#include "gfxengine/platform.hpp"

extern std::unique_ptr<Platform> _create_platform();

void startup()
{
	auto platform = _create_platform();
	auto app = create_application(*platform);
	app->start();
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
