// source: https://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 nearPoint;
out vec3 farPoint;

uniform mat4 viewInv;
uniform mat4 projInv;

vec3 UnprojectPoint(float x, float y, float z) {
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 p = aPos;
    nearPoint = UnprojectPoint(p.x, p.y, 0.0).xyz; // unprojecting on the near plane
    farPoint = UnprojectPoint(p.x, p.y, 1.0).xyz; // unprojecting on the far plane
    gl_Position = vec4(p, 1.0); // using directly the clipped coordinates
}