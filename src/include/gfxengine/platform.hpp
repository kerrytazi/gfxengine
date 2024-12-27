#pragma once

#include <memory>

class BaseWindow;
class CreateWindowParams;

class BasePlatform
{
public:

	virtual ~BasePlatform() = default;

	[[nodiscard]]
	virtual std::unique_ptr<BaseWindow> create_window(CreateWindowParams const &params) const = 0;

	[[nodiscard]]
	virtual double get_time() const = 0;
};
