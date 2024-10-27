// VERTEX
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUVS;
layout(location = 0) in vec3 aNormal;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uTransform;

out vec2 vUVs;
out vec3 vNormal;

void main() {
    vUVs = aUVS;
    vNormal = aNormal;
    gl_Position = uProjection * uView * uTransform * vec4(aPosition, 1.0);
}

// FRAGMENT
#version 330 core

in vec2 vUVs;
in vec3 vNormal;

uniform sampler2D uColorTexture;
uniform vec4 uColor;

// light structs
struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

DirectionalLight directionalLight;

out vec4 aColor;

void main() {
    vec3 lightDir = normalize(-directionalLight.direction);
    vec3 norm = normalize(vNormal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(0, 125, 255);

    vec3 ambient = vec3(0, 125, 255);
    vec4 color = uColor * texture2D(uColorTexture, vUVs);
    vec3 result = (ambient + diffuse) * color.xyz;
    aColor = vec4(result, 1.0);
}