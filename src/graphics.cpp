#include "gfxengine/graphics.hpp"

#include "gfxengine/frame.hpp"

#include <glad/glad.h>

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

class OpenGLGraphics : public BaseGraphics
{
private:

	GLuint vertex_array_object;
	GLuint vertex_buffer_object;
	GLuint element_buffer_object;

	GLuint shader_program;

	GLuint texture;

	GLuint mvp_index;
	GLuint ourTexture_index;


public:

	OpenGLGraphics()
	{
		(void)OpenGLStatic::get_singleton();

		glViewport(0, 0, 1000, 1000);

		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);

		glGenBuffers(1, &vertex_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

		glGenBuffers(1, &element_buffer_object);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);

		char const * const vertex_shader_source = R"tag(
#version 460 core

in vec3 inPos;
in vec4 inColor;
in vec2 inTexCoord;

uniform mat4 mvp;

out vec4 varColor;
out vec2 varTexCoord;

void main()
{
	gl_Position = mvp * vec4(inPos, 1.0);
	varColor = inColor;
	varTexCoord = inTexCoord;
}
)tag";

		char const * const fragment_shader_source = R"tag(
#version 460 core

in vec4 varColor;
in vec2 varTexCoord;

out vec4 outFragColor;

uniform sampler2D ourTexture;

void main()
{
	outFragColor = texture(ourTexture, varTexCoord) * varColor;
}
)tag";

		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
		glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);

		glCompileShader(vertex_shader);
		glCompileShader(fragment_shader);

		if (GLint success; glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success), !success)
		{
			char buff[1024];
			glGetShaderInfoLog(vertex_shader, sizeof(buff), nullptr, buff);
			throw 1;
			// std::cout << "glCompileShader vertexShader error: " << buff << "\n";
		}

		if (GLint success; glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success), !success)
		{
			char buff[1024];
			glGetShaderInfoLog(fragment_shader, sizeof(buff), nullptr, buff);
			throw 1;
			// std::cout << "glCompileShader fragmentShader error: " << buff << "\n";
		}

		shader_program = glCreateProgram();

		glAttachShader(shader_program, vertex_shader);
		glAttachShader(shader_program, fragment_shader);

		glLinkProgram(shader_program);

		if (GLint success; glGetProgramiv(shader_program, GL_LINK_STATUS, &success), !success)
		{
			char buff[1024];
			glGetProgramInfoLog(shader_program, sizeof(buff), nullptr, buff);
			throw 1;
			// std::cout << "glLinkProgram error: " << buff << "\n";
		}

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		{
			GLuint index = glGetAttribLocation(shader_program, "inPos");
			glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void const *>(offsetof(Vertex, pos)));
			glEnableVertexAttribArray(index);
		}

		{
			GLuint index = glGetAttribLocation(shader_program, "inColor");
			glVertexAttribPointer(index, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), reinterpret_cast<void const *>(offsetof(Vertex, color)));
			//glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void const *>(offsetof(Vertex, color)));
			glEnableVertexAttribArray(index);
		}

		{
			GLuint index = glGetAttribLocation(shader_program, "inTexCoord");
			glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void const *>(offsetof(Vertex, tex)));
			glEnableVertexAttribArray(index);
		}

		mvp_index = glGetUniformLocation(shader_program, "mvp");
		ourTexture_index = glGetUniformLocation(shader_program, "ourTexture");

		glBindFragDataLocation(shader_program, 0, "outFragColor");

		glUseProgram(shader_program);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		glEnable(GL_DEPTH_TEST);

		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	~OpenGLGraphics()
	{
		glDeleteTextures(1, &texture);
		glDeleteProgram(shader_program);
		glDeleteBuffers(1, &element_buffer_object);
		glDeleteBuffers(1, &vertex_buffer_object);
		glDeleteVertexArrays(1, &vertex_array_object);
	}

	virtual void draw(Frame const &frame) override
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(frame.data.vertices[0]) * frame.data.vertices.size(), frame.data.vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(frame.data.indices[0]) * frame.data.indices.size(), frame.data.indices.data(), GL_STATIC_DRAW);

		glUniformMatrix4fv(mvp_index, 1, GL_FALSE, &glm::identity<glm::mat4>()[0][0]);

		std::shared_ptr<Image> last_img;

		for (auto const &task : frame.tasks)
		{
			std::visit([&](auto &content) {
				using T = std::decay_t<decltype(content)>;

				if constexpr (std::is_same_v<T, DrawTaskTypes::DrawIndices>)
				{
					if (auto img = content.texture.lock(); img != last_img)
					{
						glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data.data());
						// glGenerateMipmap(GL_TEXTURE_2D);

						glUniform1i(ourTexture_index, 0);

						last_img = img;
					}

					glDrawElements(GL_TRIANGLES, content.count, GL_UNSIGNED_INT, reinterpret_cast<void const *>(content.from * sizeof(frame.data.indices[0])));
				}
				else
				if constexpr (std::is_same_v<T, DrawTaskTypes::UpdateMVP>)
				{
					glUniformMatrix4fv(mvp_index, 1, GL_FALSE, &content.mvp[0][0]);
				}
				else
				if constexpr (std::is_same_v<T, DrawTaskTypes::ClearBackground>)
				{
					glClearColor(content.color.r/255.0f, content.color.g/255.0f, content.color.b/255.0f, content.color.a/255.0f);
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
	}
};

std::unique_ptr<BaseGraphics> _create_graphics()
{
	return std::make_unique<OpenGLGraphics>();
}
