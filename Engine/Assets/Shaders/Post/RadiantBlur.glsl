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
uniform float uIntensity;  // Controls the blend between original and blurred
uniform float uBlurRadius; // Increased radius of the blur
uniform float uBlurStrength; // Control how strong the blur is
uniform int uSamples; // Control how strong the blur is

out vec4 oColor;

void main() {
    vec2 screenCenter = vec2(0.5, 0.5);
    vec2 offset = vUVs - screenCenter;
    float distanceFromCenter = length(offset);

    vec4 pixel = texture(uScreenTexture, vUVs);

    // Simple radial blur effect: sampling the texture multiple times based on distance
    vec4 blurredPixel = vec4(0.0);
    float totalWeight = 0.0;
    float blurAmount = smoothstep(0.0, uBlurRadius, distanceFromCenter) * uBlurStrength;

    // Sample in a circular pattern around the current pixel
    for(int x = -uSamples; x <= uSamples; ++x) { // Increased range for more sampling
        for(int y = -uSamples; y <= uSamples; ++y) {
            vec2 sampleUVs = vUVs + vec2(x, y) * blurAmount * 0.1; // Increased multiplier
            vec4 samplePixel = texture(uScreenTexture, sampleUVs);
            float weight = 1.0 / (1.0 + length(vec2(x, y)));
            blurredPixel += samplePixel * weight;
            totalWeight += weight;
        }
    }

    blurredPixel /= totalWeight;

    // Mix original pixel with blurred pixel based on uIntensity
    oColor = mix(pixel, blurredPixel, uIntensity);
}
