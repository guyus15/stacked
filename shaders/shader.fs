#version 330 core
in vec2 TexCoords;

out vec4 colour;

uniform sampler2D u_texture;
uniform vec4 u_colour;
uniform float u_radius;

void main()
{
    vec2 dimensions = vec2(100, 100);
    vec2 coords = TexCoords * dimensions;

    vec2 cornerDist = min(coords, dimensions - coords);
    if (cornerDist.x < u_radius && cornerDist.y < u_radius) {
        float dist = length(cornerDist - vec2(u_radius));
        if (dist > u_radius)
            discard;
    }

    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_texture, TexCoords).r);
    colour = u_colour * sampled;
}
