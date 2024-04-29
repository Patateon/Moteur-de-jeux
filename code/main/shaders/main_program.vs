#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texCoord;

// Values that stay constant for the whole mesh.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertex_ws_vs;
out vec3 normal_vs;
out vec2 texCoord_vs;
out vec3 lightPos_vs;

void main(){

    // Send normal and texture coordinates to fragment shader
    normal_vs = vertex_normal;
    texCoord_vs = vertex_texCoord;

    // Output position of the vertex, in clip space : MVP * position
    vec4 world_position = model * vec4(vertex_position_modelspace.xyz, 1);
    vertex_ws_vs = world_position.xyz;
    gl_Position = projection * view * world_position;
}
