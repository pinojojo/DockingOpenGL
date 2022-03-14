#version 330 core
out vec4 o_color;

in vec2 g_texrure_coord;
in float g_face_id;

void main()
{
	//o_color=vec4(1.0,1.0,0.0,1.0);
	o_color=vec4(g_face_id/5.0,0,0,1.0);
}