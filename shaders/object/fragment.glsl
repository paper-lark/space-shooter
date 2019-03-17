#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
    vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 texCoords;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform sampler2D aTexture;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
	// calculate ambient lighting
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

	// calculate diffuse lighting
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

    // calculate specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

    // calculate emission lighting
    vec3 emission = vec3(texture(material.emission, texCoords));

	vec4 light = vec4(ambient + diffuse + specular + emission, 1.f);
    FragColor = texture(aTexture, texCoords) * light;
}