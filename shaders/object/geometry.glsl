#version 330 core
#define EXPLOSION_SPEED 1.25f

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 fragPos;
} gs_in[];

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform uint health;
uniform float timeSinceDeath;

vec4 explode(vec4 position, vec3 normal) {
    if (health == 0u) {
        vec3 direction = normal * (exp(EXPLOSION_SPEED * timeSinceDeath) - 1);
        return position + vec4(direction, 0.f);
    }
    return position;
}

vec3 getNormal() {
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {
    vec3 explodeDir = getNormal();

    gl_Position = explode(gl_in[0].gl_Position, explodeDir);
    TexCoords = gs_in[0].texCoords;
    Normal = gs_in[0].normal;
    FragPos = gs_in[0].fragPos;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, explodeDir);
    TexCoords = gs_in[1].texCoords;
    Normal = gs_in[1].normal;
    FragPos = gs_in[1].fragPos;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, explodeDir);
    TexCoords = gs_in[2].texCoords;
    Normal = gs_in[2].normal;
    FragPos = gs_in[2].fragPos;
    EmitVertex();

    EndPrimitive();
}