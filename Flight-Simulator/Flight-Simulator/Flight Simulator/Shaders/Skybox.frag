#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform float skyNight;

void main()
{
    FragColor = skyNight*texture(skybox, TexCoords);
}