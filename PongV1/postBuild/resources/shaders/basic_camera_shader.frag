#version 410 core
out vec4 outColor;

uniform vec4 col = vec4(1.0);
void main() {
    outColor = col;
}