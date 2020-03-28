#version 330 core

in vec2 uv;
out vec4 FragColor;

// uniform float stepSize = 1.f;
uniform float gamma = 2.2;  
uniform int useBloom;
uniform int useSSAO;
uniform float exposure;


uniform sampler2D tex_position;
uniform sampler2D tex_normal;
uniform sampler2D tex_albedo;
uniform sampler2D tex_bloom;
uniform sampler2D tex_ssao;


struct Light {
    vec3 position;
    vec3 color;
    vec3 attenuation;
};
uniform Light LIGHTS[4];
uniform int nLights=1;
void main()
{    
    // vec2 texCoord = uv;
    Light light = LIGHTS[0];

    vec3 FragPos = texture(tex_position, uv).rgb;
    vec3 Normal = texture(tex_normal, uv).rgb;
    vec3 BloomColor = texture(tex_bloom, uv).rgb;
    vec4 Albedo = texture(tex_albedo, uv);
    vec3 Diffuse = Albedo.rgb;
    float AmbientOcclusion = texture(tex_ssao, uv).r * (useSSAO) + (1- useSSAO);
    

    vec3 Ambient = vec3(0.2f * Diffuse * AmbientOcclusion);
    vec3 lighting  = Ambient;   
    vec3 viewDir  = normalize(-FragPos); // viewpos is (0.0.0) 
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * light.color;


    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 8.0);
    vec3 specular = light.color * spec;

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / ( 1.0 + light.attenuation.y * distance + light.attenuation.z * distance * distance);

    diffuse *= attenuation;
    specular *= attenuation;
    lighting += diffuse + specular;

    lighting += useBloom * BloomColor; // additive blending
    

    vec3 mapped = lighting / (lighting + vec3(1.0));
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
    FragColor.rgb = mapped ;







     
    // vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    // if(bloom)
    //     hdrColor +=  lighting += bloomColor; // additive blending
    // // tone mapping
    // vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // // also gamma correct while we're at it       
    // result = pow(result, vec3(1.0 / gamma));
}