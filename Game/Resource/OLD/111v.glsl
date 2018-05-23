#version 400  
layout (location = 0) in vec3 VertexPosition;    
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexcoord;    
 
out vec3 TotalLightFront;    
out vec3 TotalLightBack;  
out vec2 TexCoords; 

const int MAX_POINT_LIGHTS = 5; 
const int MAX_SPOT_LIGHTS = 5; 

struct PointLight  
{  
    vec3 pos;  
    vec3 color; 
    float intensity;  
    float range;  
    float constant;  
    float linear;  
    float quadratic;    
};  

struct SpotLight  
{  
    vec3 pos;  
    vec3 color; 
    float intensity;  
    vec3 direction;  
    float exponent;  
    float cutoff;     
};

struct DirectionLight{  
    vec4 direction;  
    vec3 intensity;  
};
  
struct MaterialInfo{    
    vec3 diffuse;    
    vec3 specular;    
    float shininess;    
};   

uniform mat4 MVP; 
uniform mat4 MV;
uniform vec3 CameraPosition; 
uniform vec3 Ambient;
uniform int PointLightCount;   
uniform PointLight PointLights[MAX_POINT_LIGHTS]; 
uniform DirectionLight TheDirectionLight;
uniform int SpotLightCount; 
uniform SpotLight SpotLights[MAX_SPOT_LIGHTS]; 
uniform MaterialInfo Material; 

uniform mat3 NormalMatrix;  

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)  
{  
    vec3 lightDir = normalize(light.pos - fragPos);   //s 
    float diffuseFactor = max(dot(lightDir, normal), 0.0) * light.intensity; //sDotN 
    vec3 reflectDir = normalize(reflect(-lightDir, normal));  //r
    float specularFactor = pow(max(dot(reflectDir, -viewDir), 0.0), Material.shininess) * light.intensity;   
    float distance = length(light.pos - fragPos);  
    float attenuation = 1.0f;  
    if(distance < light.range)  
    {  
        attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    }   
    vec3 ambientColor = Ambient;  
    vec3 diffuseColor = diffuseFactor * Material.diffuse * light.color;  
    vec3 specularColor = specularFactor * Material.specular * light.color ;  
    return ambientColor + attenuation * (diffuseColor + specularColor);
}  

vec3 calculateDirectionLight(vec3 normal, vec3 position)  
{  
    vec3 s;  
    if(TheDirectionLight.direction.w == 0.0)  
        s = normalize(vec3(TheDirectionLight.direction));  
    else  
        s = normalize(vec3(TheDirectionLight.direction) - position);  
    vec3 v = normalize(-position);  
    vec3 r = reflect(-s, normal);  
  
    return TheDirectionLight.intensity * (Material.diffuse + Material.diffuse*max(dot(s,normal), 0.0) +   
           Material.specular * pow(max(dot(r,v),0.0), Material.shininess));  
} 

vec3 calculateSpotLight(SpotLight light, vec3 position, vec3 norm)  
{  
    vec3 s = normalize(light.pos - VertexPosition);  
    float cosTheta = dot(-s, normalize(light.direction)); 
    float cosCutoff = cos(radians(clamp(light.cutoff, 0.0, 90.0)));   
    vec3 ambient = light.intensity * Material.diffuse;  
      
    if(cosCutoff < cosTheta){  
        float spotFactor = max(pow(cosTheta, light.exponent), 0);  
        vec3 v = normalize(vec3(-position));  
        vec3 h = normalize(v + s); 
        return ambient + spotFactor * light.intensity * (Material.diffuse * max(dot(s, norm), 0.0) + Material.specular * pow(max(dot(h,norm), 0.0), Material.shininess));   
    }  
    else  
    {  
        return ambient;   
    }  
             
}  

void main()  
{
    vec3 position = vec3(MV * vec4(VertexPosition, 1.0)); 
	vec3 normal = normalize(NormalMatrix * VertexNormal);  
    vec3 viewDir = normalize(CameraPosition - VertexPosition);  
    
    vec3 totalLightFront = vec3(0.0f, 0.0f, 0.0f);  
    vec3 totalLightBack = vec3(0.0f, 0.0f, 0.0f);  
    for(int i = 0; i < PointLightCount; i++)  
    {  
        totalLightFront += calculatePointLight(PointLights[i], normal, position, viewDir);  
        totalLightBack += calculatePointLight(PointLights[i], -normal, position, viewDir);
    }
    totalLightFront += calculateDirectionLight(normal, position);  
    totalLightBack += calculateDirectionLight(-normal, position); 
    for(int i = 0; i < SpotLightCount; i++)  
    {  
        totalLightFront += calculateSpotLight(SpotLights[i], position, normal);  
        totalLightBack += calculateSpotLight(SpotLights[i], position, -normal);
    }

    TexCoords = VertexTexcoord; 
    TotalLightFront = totalLightFront;
    TotalLightBack = totalLightBack;
    gl_Position = MVP * vec4(VertexPosition, 1.0);   
} 