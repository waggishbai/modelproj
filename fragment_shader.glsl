#version 330 core

out vec4 out_color;

in vec4 pos;

float random(vec2 st)
{
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453123);
}
void main() {
    out_color = pos * .5 + .5;

}
