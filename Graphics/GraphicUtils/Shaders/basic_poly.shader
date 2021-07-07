#shader vertex
#version 330 core
layout(location = 0) in vec3 vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vertex, 1.0);
};


#shader fragment
#version 330 core
out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}