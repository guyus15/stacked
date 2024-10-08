#version 400 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_texture_coords;

out vec2 v_texture_coords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(a_position, 0.0, 1.0);
    v_texture_coords = a_texture_coords;
}
