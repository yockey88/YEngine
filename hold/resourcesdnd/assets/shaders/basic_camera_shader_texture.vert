#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoords;
out vec2 uvs;
out vec3 color;
out vec3 pos;

uniform vec3 inColor = vec3(0.0);
uniform mat4 proj = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 model = mat4(1.0);
void main() {
    color = inColor;
    pos = position;
    uvs = texcoords;
    gl_Position = proj * view * model * vec4(position , 1.0);
}