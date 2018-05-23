#version 400
flat in vec3 FrontLightIntensity;  
flat in vec3 BackLightIntensity; 
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D TextureID;

void main()
{
    if(gl_FrontFacing) FragColor = vec4(FrontLightIntensity, 1.0);//texture(TextureID, TexCoords) * vec4(FrontLightIntensity, 1.0);
    else FragColor = vec4(BackLightIntensity, 1.0);//texture(TextureID, TexCoords) * vec4(BackLightIntensity, 1.0);
}