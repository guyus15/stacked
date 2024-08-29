#version 400 core

in vec2 v_texture_coords;

out vec4 colour;

uniform sampler2D u_texture;
uniform vec2 u_size;
uniform vec4 u_colour;
uniform float u_radius;

void main()
{
    vec2 coords = v_texture_coords * u_size;

    vec2 corner_dist = min(coords, u_size - coords);
    if (corner_dist.x < u_radius && corner_dist.y < u_radius) {
        float dist = length(corner_dist - vec2(u_radius));
        if (dist > u_radius)
            discard;
    }

    colour = u_colour * vec4(1.0, 1.0, 1.0, texture(u_texture, v_texture_coords).r);
}
