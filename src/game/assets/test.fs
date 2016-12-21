#version 330 core

in vec2 UV;
in vec3 fragmentColor;

out vec3 color;

uniform sampler2D texSampler;

void main(){
    color = texture(texSampler, UV).rgb; // * fragmentColor;
}