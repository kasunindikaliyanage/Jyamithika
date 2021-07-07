#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "glad\glad.h"
#include "GLFW\glfw3.h"

class ShaderProgram {
	unsigned int shader;

	struct shader_program_source {
		std::string vertex_source;
		std::string fragment_source;
	};

	shader_program_source parseShader(const std::string & file_path)
	{
		std::ifstream stream(file_path);

		enum class shader_type
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		shader_type type;

		while (std::getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = shader_type::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = shader_type::FRAGMENT;
				}
			}
			else
			{
				ss[(int)type] << line << '\n';
			}
		}

		return { ss[0].str(), ss[1].str() };
	}

	unsigned int compilerShader(unsigned int type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Fail to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader " << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	int createShader(const std::string& vertexShader, const std::string & fragmentShader)
	{
		unsigned int program = glCreateProgram();
		unsigned int vs = compilerShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = compilerShader(GL_FRAGMENT_SHADER, fragmentShader);

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);
		return program;
	}

public:

	ShaderProgram(const std::string& file_path);

	void deleteShader();

	int getShaderId();

	unsigned int getUniformId(const char* uniform_name);

	void activeAsCurrentShader();
};