#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_emissive1;
    float shininess;
};
uniform Material material;

in vec2 TexCoords;
out vec4 color;

void main()
{
    color = vec4(texture(material.texture_diffuse1, TexCoords));
}