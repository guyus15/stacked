#version 400 core

in vec2 v_texture_coords;

out vec4 colour;

uniform sampler2D u_texture;
uniform vec4 u_colour;

void main()
{
    colour = u_colour * vec4(1.0, 1.0, 1.0, texture(u_texture, v_texture_coords).r);
}