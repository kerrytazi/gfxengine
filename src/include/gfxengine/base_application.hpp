#pragma once

#include <memory>
#include <ranges>

class Platform;
class Window;
class Frame;

class Application
{
protected:

	friend void startup();
	virtual void start()
	{
	}

public:

	virtual ~Application() = default;
};

extern std::unique_ptr<Application> create_application(Platform &platform);
