#version 330 core
layout (location = 0) in vec3 aCenter;

uniform mat4 view;
uniform mat4 projection;
uniform float radius;

uniform vec3 aabbMin;     // Minimum corner of AABB
uniform vec3 aabbMax;     // Maximum corner of AABB

out vec3 worldCenter;
out vec3 vColor;

vec3 scalingFactor = vec3(1.0);

void main() {
    worldCenter = aCenter;
    gl_Position = projection * view * vec4(aCenter, 1.0);
    gl_PointSize = radius * 100.0 / gl_Position.w; // Size in screen space

    vec3 normalized = (aCenter - aabbMin) / (aabbMax - aabbMin);

    vColor = normalized * scalingFactor; 
}
