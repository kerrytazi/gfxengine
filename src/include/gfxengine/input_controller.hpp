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

	glm::vec3 position{};
	glm::vec2 rotation{};

	enum class Direction : uint8_t
	{
		Left,        // -x (west)
		Right,       // +x (east)
		Down,        // -y
		Up,          // +y
		Forward,     // -z (north)
		Back,        // +z (south)
	};

	static inline const size_t DIRECTION_MAX = 6;

	static inline Direction DirectionList[DIRECTION_MAX]
	{
		Direction::Left,
		Direction::Right,
		Direction::Down,
		Direction::Up,
		Direction::Forward,
		Direction::Back,
	};

	InputController(InputControllerOptions _options = {})
		: options{ _options }
	{
	}

	void update_all(double time)
	{
		for (Direction direction : DirectionList)
			if (double last_time = last_times[(int)direction]; last_time != 0.0 && last_time != time)
				process_move(time, direction);
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

	void rotate_view(double event_time, glm::vec2 _rotation)
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

	glm::vec3 calc_front_direction() const
	{
		return glm::vec3{
			cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x)),
			sin(glm::radians(rotation.x)),
			sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x)),
		};
	}

private:

	void process_move(double time, Direction direction)
	{
		position += directions[(int)direction] * speed * (time - last_times[(int)direction]);
		last_times[(int)direction] = time;
	}

	void update_front_direction()
	{
		glm::vec3 direction = calc_front_direction();

		glm::vec3 front = glm::normalize(glm::vec3{ direction.x, 0.0f, direction.z });
		glm::vec3 up = glm::vec3{ 0.0f, 1.0f, 0.0f };
		glm::vec3 right = glm::normalize(glm::cross(front, up));

		directions[(int)Direction::Forward] = front;
		directions[(int)Direction::Back] = -front;
		directions[(int)Direction::Left] = -right;
		directions[(int)Direction::Right] = right;
		directions[(int)Direction::Up] = up;
		directions[(int)Direction::Down] = -up;
	}

	glm::vec3 directions[DIRECTION_MAX]{};
	double last_times[DIRECTION_MAX]{};

	float speed = 1.0f;
	float sensitivity = 1.0f;

	InputControllerOptions options;
};
