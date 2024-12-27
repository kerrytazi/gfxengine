#pragma once

#include <memory>
#include <ranges>

class BasePlatform;
class BaseWindow;
class Frame;

class BaseApplication
{
protected:

	friend void startup();
	virtual void start()
	{
	}

public:

	virtual ~BaseApplication() = default;
};

extern std::unique_ptr<BaseApplication> create_application(BasePlatform &platform);
