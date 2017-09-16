#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 particlePos;

out vec4 FragColor;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Derive camera up and right vectors in world space from the view matrix
    vec3 cameraUp = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraRight = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 p = particlePos + cameraRight * position.x + cameraUp * position.y;
    gl_Position = projection * view * vec4(p, 1.0f);
    FragColor = color;
}