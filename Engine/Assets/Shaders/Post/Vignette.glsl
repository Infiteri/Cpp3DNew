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
uniform float uFalloff;
uniform float uAmount;

out vec4 oColor;

void main() {
    vec4 color = texture2D(uScreenTexture, vUVs);

    float dist = distance(vUVs, vec2(0.5, 0.5));
    color.rgb *= smoothstep(0.8, uFalloff * 0.799, dist * (uAmount + uFalloff));

    oColor = color;
}
