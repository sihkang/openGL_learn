#version 410 core

in vec4 vPos;
in vec4 vNorm;
struct Light
{
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 att;
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shineness;
};

uniform Light light;
uniform Material material;

out vec4 fragColor;

void main()
{
    
    vec3 N = normalize(vNorm.xyz);
    vec3 L = normalize(light.position.xyz - vPos.xyz);
    vec3 V = vec3(0.0f, 0.0f, 1.0f);
    vec3 R = reflect(-L,N);

    vec4 ambient = light.ambient * material.ambient;
    float d = length(light.position.xyz - vPos.xyz);
    float denom = light.att.x + light.att.y * d + light.att.z * d * d;
    vec4 diffuse = max(dot(L,N), 0.0) * light.diffuse * material.diffuse / denom;
    vec4 specular = pow(max(dot(R, V), 0.0), material.shineness) * light.specular * material.specular / denom;

    vec4 vColor = ambient + diffuse + specular;

    fragColor = vColor;
}