// VERTEX
#version 400

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUVs;
layout(location = 2) in vec3 aNormal;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uTransform;

out vec2 vUVs;
out vec3 vNormal;
out vec3 vFragPos;

void main() {
    vUVs = aUVs;
    vNormal = mat3(transpose(inverse(uTransform))) * aNormal;
    vFragPos = vec3(uTransform * vec4(aPosition, 1.0));
    gl_Position = uProjection * uView * uTransform * vec4(aPosition, 1.0);
}

// FRAGMENT
#version 330 core

in vec2 vUVs;
in vec3 vNormal;
in vec3 vFragPos;

struct DirectionalLight {
    vec4 color;
    vec3 direction;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec4 color;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float radius;
    float intensity;
};

uniform vec4 uColor;
uniform sampler2D uColorTexture;

uniform DirectionalLight directionalLight;

// light
#define CE_NR_POINT_LIGHTS 24
uniform PointLight pointLights[CE_NR_POINT_LIGHTS];
uniform int uPointLightCount;

// World related
uniform vec3 uCameraPosition;

out vec4 oColor;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 diffuse = diff * light.color.rgb * light.intensity;

    return diffuse;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    attenuation *= (1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance))) * light.intensity;

    vec3 diffuse = light.color.rgb * diff * light.intensity;
    vec3 specular = light.specular * spec;

    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular);
}

void main() {
    vec3 normal = normalize(vNormal);
    vec3 viewDirection = normalize(uCameraPosition - vFragPos);

    vec3 directionalLightResult = CalcDirLight(directionalLight, normal, viewDirection);
    vec3 pointLightResult = vec3(0);

    for(int i = 0; i < uPointLightCount; i++) {
        pointLightResult += CalculatePointLight(pointLights[i], normal, vFragPos, viewDirection);
    }

    vec4 objectColor = texture(uColorTexture, vUVs) * uColor;
    vec4 result = vec4(1.0, 1.0, 1.0, 1.0); // Initialize with some base color, if needed
    result += vec4(directionalLightResult, 1.0);
    result += vec4(pointLightResult, 1.0);
    result *= objectColor;

    oColor = result;

}
