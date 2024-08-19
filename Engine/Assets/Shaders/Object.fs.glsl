#version 330 core

in vec2 vUVs;

uniform sampler2D uColorTexture;
uniform vec4 uColor;

out vec4 aColor;

void main() {
    aColor = uColor * texture2D(uColorTexture, vUVs);
}