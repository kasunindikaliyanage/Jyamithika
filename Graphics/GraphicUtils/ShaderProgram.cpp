#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string& file_path)
{
	shader_program_source source = parseShader(file_path);

	//std::cout << source.fragment_source << std::endl;
	shader = createShader(source.vertex_source, source.fragment_source, source.geometry_shader);
}

void ShaderProgram::deleteShader()
{
	glDeleteProgram(shader);
}

int ShaderProgram::getShaderId()
{
	return shader;
}

unsigned int ShaderProgram::getUniformId(const char* uniform_name)
{
	return glGetUniformLocation(shader, uniform_name);
}

void ShaderProgram::activeAsCurrentShader()
{
	glUseProgram(shader);
}