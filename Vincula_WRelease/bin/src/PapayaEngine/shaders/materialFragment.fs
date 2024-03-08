#version 330 core
out vec4 FragColor;

/*struct Material { // material struct
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; */

struct DirLight {
    vec3 direction;
  
    vec3 ambient;          // the vec3 are the colors of the light
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;


struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;        // with this, we get the max distance of this light
    float quadratic;          // in (https://learnopengl.com/Lighting/Light-casters) we have the data

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define MAX_POINT_LIGHTS 16
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int sizePLights;



in vec3 FragPos;  
in vec3 Normal; 
in vec2 TexCoords; 
in vec4 FragPosLightSpace;
in vec4 FragPosWorldSpace;
  
uniform float shininess;
uniform vec3 objectColor;
uniform vec3 viewPos;
//uniform Material material;
uniform sampler2D ourTexture;
uniform sampler2D shadowMap; //depth map calculated

// Functions declarations
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float CalcShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir, FragPosLightSpace);

    // phase 2: Point lights
    for(int i = 0; i < sizePLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 

    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);   

    // phase 4: Aply object color
    result *= objectColor;
    
    // phase 5: Aply object texture
    FragColor = texture(ourTexture, TexCoords) * vec4(result, 1.0);
}



// Functions implementation

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 fragPosLightSpace)
{
    vec3 lightDir = normalize(-light.direction);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading (TEMPORALY DEACTIVATED FOR OPTIMIZATION)
    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    //vec3 specular = light.specular * spec;
    //return (ambient + diffuse + specular);

    // calculate shadow for the directional light
    float shadow = CalcShadow(fragPosLightSpace, normal, lightDir);

    vec3 lighting = ambient + shadow * (diffuse /*+ specular*/);
    return lighting;
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));    

    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 



float CalcShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)   // 1.0-> no shadow / 0.0-> shadow
{
    // get vec3 of the perspective
    vec3 projCoords = fragPosLightSpace.xyz;

    // transform coordinates to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth value from light's perspective
    //float lightDepth = texture(shadowMap, projCoords.xy).r; 

    // get depth of current fragment
    float currentDepth = projCoords.z;


    // check whether current frag pos is in shadow
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0008); // To solve shadow acne
    float shadow = 1.0;
    // PCF
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y) // x, y coordinates of the texture from -1 to 1
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            if(currentDepth - bias > pcfDepth)
                shadow -= 0.1;  // from 1 to 0 in 10 points
        }    
    }


    return shadow;
} 