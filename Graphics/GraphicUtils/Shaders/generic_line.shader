#shader vertex
#version 330 core
layout(location = 0) in vec2 vertex;

void main()
{
	gl_Position = vec4(vertex, 0.0, 1.0);
};


#shader fragment
#version 330 core

out vec4 FragColor;
uniform vec3 line_color;

void main()
{
	FragColor = vec4(line_color, 1.0f);
}