#version 410 core
in vec2 uvs;
in vec4 col;
out vec4 outColor;

uniform sampler2D tex;
void main() {
    outColor = texture(tex , uvs) + col;
}