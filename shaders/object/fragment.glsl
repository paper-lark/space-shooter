#version 330 core

in vec2 texCoords;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform sampler2D aTexture;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
	float ambientStrength = 0.25f;
	float specularStrength = 0.5f;
	int shininess = 32;

	// calculate ambient lighting
	float ambient = ambientStrength;

	// calculate diffuse lighting
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diffuse = max(dot(norm, lightDir), 0.0);

    // calculate specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specular = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec4 light = vec4((ambient + diffuse + specular) * lightColor, 1.f);
    FragColor = texture(aTexture, texCoords) * light;
}