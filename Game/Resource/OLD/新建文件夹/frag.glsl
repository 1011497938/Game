#version 400  
in vec3 Position;  
flat in vec3 Normal;  
in vec2 TexCoords;

out vec4 FinalColor; 

const int MAX_POINT_LIGHTS = 5;                                                         
  
struct PointLight  
{  
    vec3 pos;  
    vec3 color; 
    float range;  
    float intensity;  
    float constant;  
    float linear;  
    float quadratic;    
};  
  
struct MaterialInfo{    
    vec3 diffuse;    
    vec3 specular;    
    float shininess;    
};    

uniform sampler2D TextureID;
uniform vec3 CameraPosition; 
uniform vec3 Ambient;
uniform int PointLightCount;   
uniform PointLight PointLights[MAX_POINT_LIGHTS];  
uniform MaterialInfo Material; 
//uniform mat4 MV;  
uniform mat3 NormalMatrix;  

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)  
{  
    vec3 lightDir = normalize(light.pos - fragPos);    
    float diffuseFactor = max(dot(normal, lightDir), 0.0) * light.intensity;   
    vec3 reflectDir = normalize(reflect(-lightDir, normal));  
    float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), Material.shininess) * light.intensity;   
    float distance = length(light.pos - fragPos);  
    float attenuation = 1.0f;  
    if(distance < light.range)  
    {  
        attenuation = 1.0f / (light.constant + light.linear * distance +   
                light.quadratic * (distance * distance));    
    }   
    vec3 ambientColor = Ambient;  
    vec3 diffuseColor = diffuseFactor * Material.diffuse * light.color;  
    vec3 specularColor = specularFactor * Material.specular * light.color ;  
    return ambientColor * Material.diffuse + diffuseColor + specularColor ;//attenuation * (diffuseColor + specularColor);
}  
  
  
void main(void)  
{  
    vec3 totalLight = vec3(0.0f, 0.0f, 0.0f);  
    vec3 normal = normalize(NormalMatrix * Normal);  
    vec3 viewDir = normalize(CameraPosition - Position);  
    if(gl_FrontFacing)
    {
        for(int i = 0; i < PointLightCount; i++)  
        {  
            totalLight += calculatePointLight(PointLights[i], normal, Position, viewDir);  
        }
    }
    else
    {
        for(int i = 0; i < PointLightCount; i++)  
        {  
            totalLight += calculatePointLight(PointLights[i], -normal, Position, viewDir);  
        }
    }

    FinalColor = vec4(totalLight, 1.0);  //texture(TextureID, TexCoords) * vec4(totalLight, 1.0);   
}  