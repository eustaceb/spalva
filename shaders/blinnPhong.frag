#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform bool blinn;
uniform int blinnPower;
uniform int phongPower;
uniform float ambientStrength;
//uniform float gamma;

float CalcShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    vec3 coords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    coords = coords / 2 + 0.5;
    if(coords.z > 1.0) // Outside frustrum
        return 0.0;
    float closestDepth = texture(shadowMap, coords.xy).r;
    float currentDepth = coords.z;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) // Similar to a gaussian kernel
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, coords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}
void main()
{
    vec3 lightColor = vec3(1.0);
    vec3 color = texture(texture1, fs_in.TexCoords).rgb;

    // Ambient
    vec3 ambient = lightColor * ambientStrength;

    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    vec3 viewDir = normalize(cameraPos - fs_in.FragPos);
    float spec = 0.0;
    if (blinn)
    {
        vec3 halfwayV = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayV), 0.0), blinnPower);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(reflectDir, viewDir), 0.0), phongPower);
    }
    vec3 specular = lightColor * spec;
    // Calc shadow
    float shadow = CalcShadow(fs_in.FragPosLightSpace, normal, lightDir);

    vec3 finalColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    //FragColor = vec4(pow(finalColor, vec3(1.0/gamma)), 1.0);
    FragColor = vec4(finalColor, 1.0f);
}