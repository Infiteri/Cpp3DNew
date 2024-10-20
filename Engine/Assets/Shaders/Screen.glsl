// VERTEX
#version 330 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aUVs;

out vec2 vUVs;

void main() {
    vUVs = aUVs;
    gl_Position = vec4(aPosition.x, aPosition.y, 0.0, 1.0);
}

// FRAGMENT
#version 330 core

in vec2 vUVs;

uniform sampler2D uScreenTexture;
bool uHalfColor;  // If true, halve the color
bool uGrayScale;  // If true, convert to grayscale

out vec4 oColor;

void main() {
    vec4 color = texture(uScreenTexture, vUVs);

    uHalfColor = false;
    uGrayScale = false;

    // Apply color halving if uHalfColor is true
    if(uHalfColor) {
        color *= 0.5;
    }

    // Apply grayscale conversion if uGrayScale is true
    if(uGrayScale) {
        color = vec4(vec3(1.0 - texture2D(uScreenTexture, vUVs)), 1.0);
    }

    oColor = color;
}
