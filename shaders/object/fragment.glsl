#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
    vec4 vector;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
    float linear;
    float quadratic;
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

	// calculate light direction
	vec3 norm = normalize(Normal);
	vec3 lightDir;
	float attenuation = 1.f;
	if (light.vector.w == 0.f) {
		// directional light
		lightDir = normalize(-vec3(light.vector));
	} else {
		// positional light
		lightDir = normalize(vec3(light.vector) - FragPos);
		float dist = length(vec3(light.vector) - FragPos);
		attenuation /= light.constant + light.linear * dist + light.quadratic * dist * dist;
	}

	// calculate diffuse lighting
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

    // calculate specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

    // calculate emission lighting
    vec3 emission = vec3(texture(material.emission, texCoords));

	vec3 light = (ambient + diffuse + specular + emission) * attenuation;
    FragColor = texture(aTexture, texCoords) * vec4(light, 1.f);
}