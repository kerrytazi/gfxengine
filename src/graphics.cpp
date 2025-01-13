#include "gfxengine/graphics.hpp"

#include "gfxengine/frame.hpp"

#include <glad/glad.h>

#include <algorithm>


static constexpr GLenum type2gltype(ShaderFieldType t)
{
	static_assert(ShaderFieldType_version == 1, "Update type2gltype::table");
	constexpr GLenum table[]{
		GL_BYTE, GL_UNSIGNED_BYTE,
		GL_SHORT, GL_UNSIGNED_SHORT,
		GL_INT, GL_UNSIGNED_INT,
		0, 0, // I64 U64
		GL_FLOAT, GL_DOUBLE,
		0, // Matrix4
		0, // Texture
	};

	return table[size_t(t)];
}

const int WIDTH = 1000;
const int HEIGHT = 1000;

class OpenGLStatic
{
private:

	OpenGLStatic()
	{
		//if (gladLoadGLLoader((GLADloadproc)load_func) == 0)
		if (gladLoadGL() == 0)
		{
			throw 1;
			// std::cout << "Failed to initialize OpenGL context\n";
		}
	}

	~OpenGLStatic()
	{
	}

	OpenGLStatic(OpenGLStatic const &) = delete;
	OpenGLStatic &operator = (OpenGLStatic const &) = delete;
	OpenGLStatic(OpenGLStatic &&) = delete;
	OpenGLStatic &operator = (OpenGLStatic &&) = delete;

public:

	static OpenGLStatic &get_singleton()
	{
		static OpenGLStatic instance;
		return instance;
	}
};

namespace OpenGL
{


template <typename T, typename FDestruct, T Default = T(-1)>
struct MoveOnly
{
	T val = Default;

	constexpr MoveOnly() = default;

	constexpr MoveOnly(T _val)
		: val{ _val }
	{
	}

	~MoveOnly()
	{
		if (val != Default)
		{
			FDestruct{}(val);
			val = Default;
		}
	}

	MoveOnly(MoveOnly const &) = delete;
	MoveOnly &operator = (MoveOnly const &) = delete;

	constexpr MoveOnly(MoveOnly &&other)
		: val{ other.val }
	{
		other.val = Default;
	}
	MoveOnly &operator = (MoveOnly &&other) noexcept
	{
		if (val != Default)
			FDestruct{}(val);

		val = other.val;
		other.val = Default;

		return *this;
	}

	operator T() const
	{
		return val;
	}

	T *operator & ()
	{
		return &val;
	}

	T const *operator & () const
	{
		return &val;
	}
};

struct VertexArray
{
	MoveOnly<GLuint, decltype([](GLuint v) { glDeleteVertexArrays(1, &v); })> vertex_array;

	VertexArray()
	{
		glGenVertexArrays(1, &vertex_array);
	}

	void bind()
	{
		glBindVertexArray(vertex_array);
	}
};

struct Buffer
{
	MoveOnly<GLuint, decltype([](GLuint v) { glDeleteBuffers(1, &v); })> buffer;
	GLenum target = GLenum(-1);

	explicit Buffer(GLenum _target)
	{
		glGenBuffers(1, &buffer);
		target = _target;
	}

	void bind()
	{
		glBindBuffer(target, buffer);
	}
};

struct Buffers
{
	VertexArray vao{};
	Buffer vbo{ GL_ARRAY_BUFFER };
	Buffer ebo{ GL_ELEMENT_ARRAY_BUFFER };
};

struct Shader
{
	MoveOnly<GLuint, decltype([](GLuint v) { glDeleteShader(v); })> shader;
	GLenum type = GLenum(-1);

	explicit Shader(GLenum _type)
	{
		shader = glCreateShader(_type);
		type = _type;
	}

	static Shader compile(GLenum _type, char const *source)
	{
		Shader result(_type);

		glShaderSource(result.shader, 1, &source, nullptr);
		glCompileShader(result.shader);

		if (GLint success; glGetShaderiv(result.shader, GL_COMPILE_STATUS, &success), !success)
		{
			char buff[1024];
			glGetShaderInfoLog(result.shader, sizeof(buff), nullptr, buff);
			throw 1;
			// std::cout << "glCompileShader vertexShader error: " << buff << "\n";
		}

		return result;
	}
};

struct Program
{
	MoveOnly<GLuint, decltype([](GLuint v) { glDeleteProgram(v); })> program;

	Program()
	{
		program = glCreateProgram();
	}

	void use()
	{
		glUseProgram(program);
	}

	GLint find_attribute(char const *name)
	{
		return glGetAttribLocation(program, name);
	}

	GLint find_uniform(char const *name)
	{
		return glGetUniformLocation(program, name);
	}

	static Program link(std::span<const Shader> shaders)
	{
		Program result;

		for (auto const &shader : shaders)
			glAttachShader(result.program, shader.shader);

		glLinkProgram(result.program);

		if (GLint success; glGetProgramiv(result.program, GL_LINK_STATUS, &success), !success)
		{
			char buff[1024];
			glGetProgramInfoLog(result.program, sizeof(buff), nullptr, buff);
			throw 1;
			// std::cout << "glLinkProgram error: " << buff << "\n";
		}

		return result;
	}
};


} // namespace OpenGL


struct OpenGLMaterial : Material
{
	OpenGL::Program program;

	std::optional<OpenGL::Buffers> buffers;

	OpenGLMaterial(OpenGL::Program _program, ShaderValuesInfo _attribute_info, ShaderValuesInfo _uniform_info)
		: program{ std::move(_program) }
	{
		attribute_info = std::move(_attribute_info);
		uniform_info = std::move(_uniform_info);
		uniforms.resize(uniform_info.fields.size());
	}
};


class OpenGLGraphics : public Graphics
{
private:

	GLuint multisample_framebuffer;
	GLuint multisample_texture_color;
	GLuint multisample_texture_depth;

	GLuint texture;

public:

	OpenGLGraphics()
	{
		(void)OpenGLStatic::get_singleton();

		glViewport(0, 0, WIDTH, HEIGHT);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);

		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


		glGenFramebuffers(1, &multisample_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, multisample_framebuffer);

		glGenTextures(1, &multisample_texture_color);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisample_texture_color);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, WIDTH, HEIGHT, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisample_texture_color, 0);

		glGenTextures(1, &multisample_texture_depth);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisample_texture_depth);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, multisample_texture_depth, 0);

		glEnable(GL_MULTISAMPLE);
	}

	~OpenGLGraphics()
	{
		glDeleteTextures(1, &multisample_texture_depth);
		glDeleteTextures(1, &multisample_texture_color);
		glDeleteFramebuffers(1, &multisample_framebuffer);

		glDeleteTextures(1, &texture);
	}

	virtual void draw(Frame const &frame) override
	{
		glBindFramebuffer(GL_FRAMEBUFFER, multisample_framebuffer);

		GLuint active_vao = (GLuint)-1;
		std::vector<std::shared_ptr<Image>> active_imgs(4);

		for (auto const &task : frame.tasks)
		{
			std::visit([&](auto &content) {
				using T = std::decay_t<decltype(content)>;

				if constexpr (std::is_same_v<T, DrawTaskTypes::DrawMaterial>)
				{
					auto gm = std::static_pointer_cast<OpenGLMaterial>(content.material);

					gm->program.use();

					if (!gm->buffers)
					{
						gm->buffers = OpenGL::Buffers{};
						gm->buffers->vao.bind();
						gm->buffers->vbo.bind();
						gm->buffers->ebo.bind();

						for (size_t i = 0, offset = 0; i < gm->attribute_info.fields.size(); ++i)
						{
							auto const &f = gm->attribute_info.fields[i];
							glVertexAttribPointer(i, f.count, type2gltype(f.type), f.normalize, gm->attribute_info.total_byte_size, (void *)offset);
							glEnableVertexAttribArray(i);
							offset += f.byte_size();
						}
					}
					else
					{
						gm->buffers->vao.bind();
					}

					active_vao = gm->buffers->vao.vertex_array.val;

					glBufferData(GL_ARRAY_BUFFER, content.vertices.size(), content.vertices.data(), GL_STATIC_DRAW);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(content.indices[0]) * content.indices.size(), content.indices.data(), GL_STATIC_DRAW);

					for (size_t i = 0, img_count = 0; i < gm->uniform_info.fields.size(); ++i)
					{
						auto const &f = gm->uniform_info.fields[i];

						static_assert(ShaderFieldType_version == 1, "Update OpenGLGraphics::draw");
						switch (f.type)
						{
							case ShaderFieldType::Matrix4:
								glUniformMatrix4fv(i, 1, GL_FALSE, &std::get<mat4>(*content.material->uniforms[i])[0][0]);
								break;

							case ShaderFieldType::Texture:
								{
									if (auto &img = std::get<ShaderFieldTexture_t>(*content.material->uniforms[i]).img; img != active_imgs[img_count])
									{
										glActiveTexture(GL_TEXTURE0 + img_count);
										glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data.data());
										// glGenerateMipmap(GL_TEXTURE_2D);
										glUniform1i(i, img_count);
										active_imgs[img_count] = img;
									}

									img_count += 1;
								}
								break;

							default:
								throw 1;
						}
					}

					// TODO
					glDrawElements(GL_TRIANGLES, content.indices.size(), GL_UNSIGNED_INT, reinterpret_cast<void const *>(0 * sizeof(content.indices[0])));
				}
				else
				if constexpr (std::is_same_v<T, DrawTaskTypes::ClearBackground>)
				{
					glClearColor(content.color.r, content.color.g, content.color.b, content.color.a);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				}
				else
				if constexpr (std::is_same_v<T, DrawTaskTypes::SettingWireFrame>)
				{
					glPolygonMode(GL_FRONT_AND_BACK, content.enable ? GL_LINE : GL_FILL);
				}
				else
				if constexpr (std::is_same_v<T, DrawTaskTypes::SettingCulling>)
				{
					if (content.enable)
					{
						glEnable(GL_CULL_FACE);
						glCullFace(GL_BACK);
						glFrontFace(GL_CCW);
					}
					else
					{
						glDisable(GL_CULL_FACE);
					}
				}
				else
				if constexpr (std::is_same_v<T, DrawTaskTypes::SettingBlend>)
				{
					throw 1;
				}
				else
				if constexpr (std::is_same_v<T, DrawTaskTypes::SettingDepth>)
				{
					if (content.enable)
					{
						glEnable(GL_DEPTH_TEST);
					}
					else
					{
						glDisable(GL_DEPTH_TEST);
					}
				}
			}, task);
		}

		glBindFramebuffer(GL_READ_FRAMEBUFFER, multisample_framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}

	virtual std::shared_ptr<Material> create_material(CreateMaterialParams const &params) override
	{
		return std::make_shared<OpenGLMaterial>(
			OpenGL::Program::link(std::initializer_list<OpenGL::Shader>{
				OpenGL::Shader::compile(GL_VERTEX_SHADER, params.vertex_shader),
				OpenGL::Shader::compile(GL_FRAGMENT_SHADER, params.fragment_shader),
			}),
			params.attributes,
			params.uniforms
		);
	}
};

std::unique_ptr<Graphics> _create_graphics()
{
	return std::make_unique<OpenGLGraphics>();
}
