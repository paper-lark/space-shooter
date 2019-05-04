#version 330 core
#define POINT_LIGHTS_COUNT 4

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_specular1;
	sampler2D texture_emission1;
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

uniform vec3 viewPos;
uniform Material material;
uniform uint health;

uniform DirectionalLight dirLight;
uniform PointLight pointLights[POINT_LIGHTS_COUNT];
uniform SpotlightLight flashlight;


vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);

   	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
   	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoords));
   	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoords));
   	vec3 emission = vec3(texture(material.texture_emission1, texCoords));
   	return max(ambient + diffuse + specular, 0.f);
}


vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - FragPos);
	float dist = length(light.position - FragPos);
	float attenuation = 1.f / (light.constant + light.linear * dist + light.quadratic * dist * dist);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoords));
	return max((ambient + diffuse + specular) * attenuation, 0.f);
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

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, texCoords));

	return max((ambient + intensity * diffuse + specular * intensity) * attenuation, 0.f);
}


void main() {

	// calculate prerequisites
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 light = texture(material.texture_emission1, texCoords).xyz;

	// calculate light
	light += calculateDirectionalLight(dirLight, normal, viewDir);

	for (int i = 0; i < POINT_LIGHTS_COUNT; i++) {
		if (pointLights[i].position == vec3(0, 0, 0)) {
			continue;
		}

		vec3 delta = calculatePointLight(pointLights[i], normal, viewDir);
		light += delta;
	}
	light += calculateFlashlightLight(flashlight, normal, viewDir);

	// calculate health level
	float healthLevel = min(health / 100.f + 0.25f, 1.f);

    FragColor = vec4(light * healthLevel, 1.f);
}