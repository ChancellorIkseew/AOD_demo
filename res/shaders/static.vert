#version 460 core
layout(location = 0) in vec3  a_pos;
layout(location = 1) in vec2  a_uv;
layout(location = 2) in vec4  a_color;   // ubyte normalized → float

uniform mat4 u_vp;        // view × projection
uniform mat4 u_model;     // per-draw
uniform vec4 u_uvRect;    // offset.xy, scale.wh
uniform vec4 u_tint;      // per-draw RGBA [0,1]

out vec2 v_uv;
out vec4 v_color;

void main() {
    gl_Position = u_vp * u_model * vec4(a_pos, 1.0);
    v_uv    = u_uvRect.xy + a_uv * u_uvRect.zw;
    v_color = a_color * u_tint;
}
