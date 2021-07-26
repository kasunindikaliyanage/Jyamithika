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

void main()
{
	FragColor = vec4(0.8, 0.2, 0.3, 1.0f);
}