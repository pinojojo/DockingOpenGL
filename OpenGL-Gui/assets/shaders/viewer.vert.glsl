#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coord;
layout (location = 2) in vec3 vertex_color;
out vec2 g_texrure_coord;
out vec3 g_vertex_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	g_texrure_coord = vec2(texture_coord.x, texture_coord.y);
	g_vertex_color=vertex_color;
}