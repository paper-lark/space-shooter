#version 330 core

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_specular1;
	sampler2D texture_emission1;
	float shininess;
};

in vec2 texCoords;
out vec4 FragColor;
uniform Material material;

void main() {
    FragColor = texture(material.texture_diffuse1, texCoords);
}