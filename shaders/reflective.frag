#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform bool mirror;
uniform bool mixColors;
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    if (mirror) R = -R;
     vec3 tex = texture(skybox, R).rgb;
    if (mixColors) tex = tex.brg;

    FragColor = vec4(tex, 1.0f);
}