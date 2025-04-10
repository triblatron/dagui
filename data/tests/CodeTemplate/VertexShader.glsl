#version 130

uniform mat4 mvp;
attribute vec2 pos;
attribute vec4 colour;

void main()
{
	gl_VertexPosition = mvp * pos;
}
