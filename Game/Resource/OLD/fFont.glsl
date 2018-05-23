#version 400    
in vec2 TexCoords;

out vec4 FinalColor;

uniform sampler2D TextureID;
uniform vec3 Color;

void main()
{
	FinalColor = vec4(1.0, 1.0, 1.0, texture(TextureID, TexCoords).r) * vec4(Color.xyz, 1.0);
}