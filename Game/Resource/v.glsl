#version 400  
layout (location = 0) in vec3 VertexPosition;    
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexcoord;   
layout (location = 3) in ivec4 BoneIDs;  
layout (location = 4) in vec4 Weights; 
 
out vec3 TotalLightFront;    
out vec3 TotalLightBack;  
out vec2 TexCoords; 
out vec4 ShadowCoord;
out vec3 AmbientColor;
out float Bias;
out vec4 PosL2;

const int MAX_POINT_LIGHTS = 5; 
const int MAX_SPOT_LIGHTS = 5; 

const int MAX_BONES = 150;  

uniform bool isAnim;

uniform mat4 gBones[MAX_BONES];  

struct DirectionLight{  
    vec4 direction;  
    vec3 intensity;  
};
  
struct MaterialInfo{    
    vec3 diffuse;    
    vec3 specular;    
    float shininess;    
};   

uniform mat4 Model; 
uniform mat4 MVP; 
uniform mat4 DepthBiasMVP;
uniform mat4 MV;
uniform vec3 CameraPosition; 
uniform DirectionLight TheDirectionLight;
uniform MaterialInfo Material; 

uniform mat3 NormalMatrix;  

vec3 calculateDirectionLight(vec3 normal, vec3 normalEye, vec3 position)  
{  
    vec3 s;  
    if(TheDirectionLight.direction.w == 0.0)  
        s = normalize(vec3(TheDirectionLight.direction));  
    else  
        s = normalize(vec3(TheDirectionLight.direction) - position);  
    vec3 v = normalize(-position);  
    vec3 r = reflect(-s, normalEye);  
  
    return TheDirectionLight.intensity * (Material.diffuse + Material.diffuse*max(dot(s,normal), 0.0) +   
           Material.specular * pow(max(dot(r,v),0.0), Material.shininess));  
} 


void main()  
{
    vec3 position = vec3(MV * vec4(VertexPosition, 1.0)); 
	vec3 normalEye = normalize(NormalMatrix * VertexNormal);
    vec3 normal = normalize(VertexNormal);  
    vec3 viewDir = normalize(CameraPosition - VertexPosition);  
    
    vec3 totalLightFront = vec3(0.0f, 0.0f, 0.0f);  
    vec3 totalLightBack = vec3(0.0f, 0.0f, 0.0f);  
  
    totalLightFront += calculateDirectionLight(normal, normalEye, position);  
    totalLightBack += calculateDirectionLight(-normal, -normalEye, position); 

    float bias = 0.005*tan(acos(dot(normalize(VertexNormal),vec3(0,0,1))));
    Bias = clamp(bias, 0.0, 0.01);
    ShadowCoord = DepthBiasMVP * vec4(VertexPosition, 1.0);
    AmbientColor = TheDirectionLight.intensity * Material.diffuse;
    TexCoords = VertexTexcoord; 
    TotalLightFront = totalLightFront;
    TotalLightBack = totalLightBack;

    mat4 BoneTransform = gBones[BoneIDs.x] * Weights.x;  
    BoneTransform += gBones[BoneIDs.y] * Weights.y;  
    BoneTransform += gBones[BoneIDs.z] * Weights.z;  
    BoneTransform += gBones[BoneIDs.w] * Weights.w;  

    vec4 PosL;
    if(isAnim)
    	PosL = BoneTransform * vec4(VertexPosition, 1.0);
    else
    	PosL = vec4(VertexPosition, 1.0);
    PosL2 = Model * PosL;

    gl_Position = MVP * PosL;   
} 