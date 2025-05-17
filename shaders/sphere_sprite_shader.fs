#version 330 core
out vec4 FragColor;

in vec3 worldCenter;

uniform vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // Directional light (in world or view space)

void main() {
    // Convert point coordinate from [0, 1] to [-1, 1]
    vec2 coord = gl_PointCoord * 2.0 - 1.0;

    float dist2 = dot(coord, coord);
    if (dist2 > 1.0)
        discard; // Outside the circle

    // Reconstruct a hemisphere normal (Z points out of screen)
    vec3 normal = normalize(vec3(coord, sqrt(1.0 - dist2)));

    // Fake lighting using a fixed light direction
    float diffuse = max(dot(normal, normalize(lightDir)), 0.0);
    vec3 baseColor = vec3(0.2, 0.8, 1.0);
    vec3 color = baseColor * diffuse;

    FragColor = vec4(color, 1.0);
}
