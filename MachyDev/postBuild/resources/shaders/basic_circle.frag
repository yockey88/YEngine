#version 410 core

in vec3 pos;

out vec4 outColor;

uniform vec4 col = vec4(1.0);
void main() {
    if (length(pos) > 0.5) discard;
    outColor = col;
}