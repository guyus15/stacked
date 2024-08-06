#version 400 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texture_coords;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(position, 0.0, 1.0);
    TexCoords = texture_coords;
}
