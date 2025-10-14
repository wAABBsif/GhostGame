layout(location = 0) in uvec3 data;

out float tex_index;
out vec2 tex_coords;
out vec4 color;

uniform mat3 world_to_screen_matrix;

void main()
{
    vec3 pos = world_to_screen_matrix * vec3(data.x & uint(0xFFFF), (data.x >> 16) & uint(0xFFFF), 1);
    gl_Position = vec4(pos.xy, 0 - float((data.z >> 16) & uint(0xFF)) / 256.0, 1);

    tex_coords = vec2(float(data.y & uint(0xFFFF)) / 65536.0, float((data.y >> 16) & uint(0xFFFF)) / 65536.0);
    tex_index = float((data.z >> 24) & uint(0xFF));
    color = vec4(float((data.z >> 12) & uint(0xF)) / 15.0, float((data.z >> 8) & uint(0xF)) / 15.0, float((data.z >> 4) & uint(0xF)) / 15.0, float((data.z >> 0) & uint(0xF)) / 15.0);
}