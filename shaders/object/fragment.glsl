#version 330 core
#define POINT_LIGHTS_COUNT 4

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};


struct DirectionalLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
    vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

struct SpotlightLight {
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outerCutOff;

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

uniform DirectionalLight dirLight;
uniform PointLight pointLights[POINT_LIGHTS_COUNT];
uniform SpotlightLight flashlight;


vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);

	// TODO: move into a separate function
   	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
   	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
   	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
   	vec3 emission = vec3(texture(material.emission, texCoords));
   	return ambient + diffuse + specular;
}


vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - FragPos);
	float dist = length(light.position - FragPos);
	float attenuation = 1.f / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);

	// TODO: move into a separate function
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
	return (ambient + diffuse + specular) * attenuation;
}

vec3 calculateFlashlightLight(SpotlightLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - FragPos);
	float dist = length(light.position - FragPos);
	float attenuation = 1.f / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);
	float theta = dot(lightDir, -light.direction);
	float eps = light.innerCutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / eps, 0.f, 1.f);

	// TODO: move into a separate function
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	return (ambient + intensity * diffuse + specular * intensity) * attenuation;
}


void main() {
	// calculate prerequisites
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec4 light = texture(material.emission, texCoords); // emission light

	// calculate light
	light += vec4(calculateDirectionalLight(dirLight, normal, viewDir), 1.f);
	for (int i = 0; i < POINT_LIGHTS_COUNT; i++) {
		light += vec4(calculatePointLight(pointLights[i], normal, viewDir), 1.f);
	}
	light += vec4(calculateFlashlightLight(flashlight, normal, viewDir), 1.f);

    FragColor = texture(aTexture, texCoords) * light;
}