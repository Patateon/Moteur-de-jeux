#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

// Output data ; will be interpolated for each fragment.
out vec3 position_worldspace;
out vec3 normal_worldspace;
out vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace, 1.0);
	
	// Position of the vertex, in worldspace : M * position
	position_worldspace = (M * vec4(vertexPosition_modelspace, 1.0)).xyz;
	
	// Normal of the the vertex, in world space
	normal_worldspace = (M * vec4(vertexPosition_modelspace, 1.0)).xyz;

	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}