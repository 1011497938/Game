#version 400  
in vec3 TotalLightFront; 
in vec3 TotalLightBack;  
in vec2 TexCoords;
in vec4 ShadowCoord;
in vec3 AmbientColor;
in float Bias;
in vec4 PosL2;

out vec4 FinalColor;

uniform sampler2D Texture_D, Texture_S;
uniform sampler2DShadow ShadowMap;
uniform vec3 CenterPos;

vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

float calculateShadowFactor(vec3 shadowCoord)
{
	float factor = 1.0;
	/*for (int i=0;i<4;i++){
		if ( textureProj( ShadowMap, vec4(shadowCoord.xy + poissonDisk[i]/2000.0, shadowCoord.z - 0.0007, 1.0) ) == 0.0 ){
			factor -= 0.2;
  		}
  	}*/
  	if(textureProj( ShadowMap, vec4(shadowCoord.xy, shadowCoord.z + 0.0001, 1.0) ) == 0.0)
  		factor = 0.2;
  	
  	/*for (int i=0;i<4;i++) {
  		float dot_product = dot(vec4(gl_FragCoord.xyy, i), vec4(12.9898,78.233,45.164,94.673));
    	int index = int(4*fract(sin(dot_product) * 43758.5453));
    	if ( textureProj( ShadowMap, vec4(shadowCoord.xy + poissonDisk[index]/2000.0, shadowCoord.z - 0.0005, 1.0) ) == 0.0 ){
			factor -= 0.2;
  		}
    }*/

    return factor;
}

void main()
{
	vec3 shadowCoord = vec3(ShadowCoord) / ShadowCoord.w;
	float factor = calculateShadowFactor(shadowCoord);
	int depthView;
	if((PosL2.x-CenterPos.x)*(PosL2.x-CenterPos.x) + (PosL2.y-CenterPos.y)*(PosL2.y-CenterPos.y) <= 1600)
    	depthView = 1;
    else
    	depthView = 0;
	if(depthView == 0)
		factor = 1.0;
	if(gl_FrontFacing)
		FinalColor = (factor * vec4(TotalLightFront, 1.0) + (1-factor) * vec4(AmbientColor, 1.0))*texture(Texture_D, TexCoords);
	else
		FinalColor = (factor * vec4(TotalLightBack, 1.0) + (1-factor) * vec4(AmbientColor, 1.0))*texture(Texture_D, TexCoords);
}