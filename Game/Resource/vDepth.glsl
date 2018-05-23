#version 400  
layout (location = 0) in vec3 VertexPosition;   
layout (location = 3) in ivec4 BoneIDs;  
layout (location = 4) in vec4 Weights; 

uniform mat4 MVP;

const int MAX_BONES = 150;  

uniform bool isAnim;

uniform mat4 gBones[MAX_BONES];  

void main()
{
	mat4 BoneTransform = gBones[BoneIDs.x] * Weights.x;  
    BoneTransform += gBones[BoneIDs.y] * Weights.y;  
    BoneTransform += gBones[BoneIDs.z] * Weights.z;  
    BoneTransform += gBones[BoneIDs.w] * Weights.w;  

    vec4 PosL;
    if(isAnim)
    	PosL = BoneTransform * vec4(VertexPosition, 1.0);
    else
    	PosL = vec4(VertexPosition, 1.0);
    gl_Position = MVP * PosL;
}