#version 330 core

layout(location = 0) in vec3 position;

uniform mat3 test;

void main()
{
	vec3 pos_2D = test * vec3(position.xy, 1);
	gl_Position = vec4(pos_2D.xy, position.z, 1);
}