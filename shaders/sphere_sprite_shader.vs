#version 330 core
layout (location = 0) in vec3 aCenter;

uniform mat4 view;
uniform mat4 projection;
uniform float radius;

out vec3 worldCenter;

void main() {
    worldCenter = aCenter;
    gl_Position = projection * view * vec4(aCenter, 1.0);
    gl_PointSize = radius * 100.0 / gl_Position.w; // Size in screen space
}
