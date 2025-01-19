#pragma once

#include "gfxengine/math.hpp"

#include <memory>
#include <vector>
#include <variant>
#include <string>
#include <optional>

struct Image;

static constexpr size_t ShaderFieldType_version = 2;

static_assert(ShaderFieldType_version == 2, "Update ShaderFieldType");
enum class ShaderFieldType : uint16_t
{
	I8, U8,
	I16, U16,
	I32, U32,
	I64, U64,
	F32, F64,

	Matrix4,
	Vec1, Vec2, Vec3, Vec4,
	Texture,
};

struct ShaderFieldTexture_t
{
	std::shared_ptr<Image> img;
};

static_assert(ShaderFieldType_version == 2, "Update ShaderFieldValue");
using ShaderFieldValue = std::variant<
	int8_t, uint8_t,
	int16_t, uint16_t,
	int32_t, uint32_t,
	int64_t, uint64_t,
	float, double,
	mat4,
	vec1, vec2, vec3, vec4,
	ShaderFieldTexture_t
>;

struct ShaderFieldInfo
{
	std::string name;
	ShaderFieldType type;
	bool normalize;
	uint32_t count;

	constexpr size_t byte_size() const
	{
		return type_size(type) * count;
	}

	static constexpr size_t type_size(ShaderFieldType t)
	{
		static_assert(ShaderFieldType_version == 2, "Update ShaderFieldInfo::type_size::table");
		constexpr size_t table[]{
			1, 1,
			2, 2,
			4, 4,
			8, 8,
			4, 8,
			4*16,
			4, 8, 12, 16,
			4,
		};

		return table[size_t(t)];
	}
};

struct ShaderValuesInfo
{
	size_t total_byte_size = 0;
	std::vector<ShaderFieldInfo> fields;

	void add(ShaderFieldInfo field)
	{
		fields.push_back(field);
		total_byte_size += field.byte_size();
	}
};

struct CreateMaterialParams
{
	char const *vertex_shader = nullptr;
	char const *fragment_shader = nullptr;
	ShaderValuesInfo attributes;
	ShaderValuesInfo uniforms;
};

struct Material
{
	virtual ~Material() = default;

	ShaderValuesInfo attribute_info;
	ShaderValuesInfo uniform_info;
	std::vector<std::optional<ShaderFieldValue>> uniforms;
};
