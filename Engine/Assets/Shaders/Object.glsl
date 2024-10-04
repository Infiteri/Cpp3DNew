// VERTEX
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUVS;
layout(location = 0) in vec3 aNormal;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uTransform;

out vec2 vUVs;

void main() {
    vUVs = aUVS;
    gl_Position = uProjection * uView * uTransform * vec4(aPosition, 1.0);
}

// FRAGMENT

#version 330 core

in vec2 vUVs;

uniform sampler2D uColorTexture;
uniform vec4 uColor;

out vec4 aColor;

void main() {
    aColor = uColor * texture2D(uColorTexture, vUVs);
}