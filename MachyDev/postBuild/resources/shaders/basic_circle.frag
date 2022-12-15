#version 410 core
in vec3 pos;
in vec3 color;

out vec4 outColor;
void main() {
    if (length(pos) > 0.5) discard;
    outColor = vec4(color , 1.0);
}