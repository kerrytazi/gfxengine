#pragma once

#include "gfxengine/math.hpp"

struct InputControllerOptions
{
	bool limit_pitch = true;
	bool invert_pitch = true;
};

class InputController
{
public:

	vec3 position{};
	vec2 rotation{};

	enum class Direction : uint8_t
	{
		Left,        // -x (west)
		Right,       // +x (east)
		Down,        // -y
		Up,          // +y
		Back,        // -z (north)
		Front,       // +z (south)
	};

	static inline const size_t DIRECTION_MAX = 6;

	static inline Direction DirectionList[DIRECTION_MAX]
	{
		Direction::Left,
		Direction::Right,
		Direction::Down,
		Direction::Up,
		Direction::Back,
		Direction::Front,
	};

	static inline uint32_t DirectionBitMask[DIRECTION_MAX]
	{
		0b001,
		0b001,
		0b010,
		0b010,
		0b100,
		0b100,
	};

	InputController(InputControllerOptions _options = {})
		: options{ _options }
	{
	}

	void update_all(double time)
	{
		vec3 delta_pos{};
		uint32_t flags = 0;

		for (Direction direction : DirectionList)
		{
			if (double last_time = last_times[(int)direction]; last_time != 0.0 && last_time != time)
			{
				flags ^= DirectionBitMask[(int)direction];
				process_move(delta_pos, time, direction);
			}
		}

		int active_directions = std::popcount(flags);

		if (active_directions > 0)
		{
			float mult = 1.0f / math::sqrt((float)active_directions);
			position += delta_pos * mult;
		}
	}

	void stop_all(double time)
	{
		update_all(time);

		for (Direction direction : DirectionList)
			last_times[(int)direction] = 0.0;
	}

	void begin_move(double event_time, Direction direction)
	{
		update_all(event_time);
		last_times[(int)direction] = event_time;
	}

	void end_move(double event_time, Direction direction)
	{
		if (last_times[(int)direction] == 0.0)
			return;

		update_all(event_time);
		last_times[(int)direction] = 0.0;
	}

	void change_speed(double event_time, float _speed)
	{
		update_all(event_time);
		speed = _speed;
	}

	void rotate_view(double event_time, vec2 _rotation)
	{
		update_all(event_time);

		if (options.invert_pitch)
			_rotation.x = -_rotation.x;

		rotation += _rotation * sensitivity;

		if (options.limit_pitch)
		{
			if (rotation.x > 89.0f)
				rotation.x =  89.0f;
			if (rotation.x < -89.0f)
				rotation.x = -89.0f;
		}

		update_front_direction();
	}

	void change_rotation_sensitivity(double event_time, float _sensitivity)
	{
		update_all(event_time);
		sensitivity = _sensitivity;
	}

	vec3 calc_front_direction() const
	{
		return vec3{
			math::cos(math::deg_to_rad(rotation.y)) * math::cos(math::deg_to_rad(rotation.x)),
			math::sin(math::deg_to_rad(rotation.x)),
			math::sin(math::deg_to_rad(rotation.y)) * math::cos(math::deg_to_rad(rotation.x)),
		};
	}

private:

	void process_move(vec3 &delta_pos, double time, Direction direction)
	{
		delta_pos += directions[(int)direction] * speed * (time - last_times[(int)direction]);
		last_times[(int)direction] = time;
	}

	void update_front_direction()
	{
		vec3 direction = calc_front_direction();

		vec3 front = vec3{ direction.x, 0.0f, direction.z }.normalize();
		vec3 up = vec3::unit_y();
		vec3 right = front.cross(up).normalize();

		directions[(int)Direction::Left] = -right;
		directions[(int)Direction::Right] = right;
		directions[(int)Direction::Down] = -up;
		directions[(int)Direction::Up] = up;
		directions[(int)Direction::Back] = -front;
		directions[(int)Direction::Front] = front;
	}

	vec3 directions[DIRECTION_MAX]{};
	double last_times[DIRECTION_MAX]{};

	float speed = 1.0f;
	float sensitivity = 1.0f;

	InputControllerOptions options;
};
