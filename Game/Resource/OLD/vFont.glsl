#version 400  
layout (location = 0) in vec3 VertexPosition;    
layout (location = 2) in vec2 VertexTexcoord; 

out vec2 TexCoords; 

uniform mat4 MVP;

void main()
{
	TexCoords = VertexTexcoord;

	gl_Position = MVP * vec4(VertexPosition, 1.0);  
}