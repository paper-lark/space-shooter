#version 330 core
layout (location = 0) in vec4 vertex; // position + texture
out vec2 TexCoords;
uniform mat4 projection;

void main() {
	gl_Position = projection * vec4(vertex.xy, 0.f, 1.f);
	TexCoords = vertex.zw;
}