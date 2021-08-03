#shader vertex
#version 330 core
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 incolor;

out vec3 tricolor;

void main()
{
	gl_Position = vec4(vertex, 1.0);
	tricolor = incolor;
};


#shader fragment
#version 330 core

in vec3 tricolor;
out vec4 FragColor;

void main()
{
	FragColor = vec4(tricolor, 0.5f);
}