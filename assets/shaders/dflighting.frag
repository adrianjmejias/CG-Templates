#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


uniform float stepSize;
uniform vec2 convSize;
uniform vec2 convPivot;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
    float Radius;
};

uniform int nLights;
const int NR_LIGHTS = 32;
uniform Light LIGHTS[NR_LIGHTS];
uniform vec3 viewPos;



/*
no se como ponerle un clear color por textura, por default lo ponia en uno
entonces lo deje asi jejejeje
*/
vec3 blur(sampler2D tex, vec2 convSize, vec2 pivot, vec2 actPos, float stepSize)
{
    // si el valor es mayor a 0.5 no le hacemos bloom
    if(texture(gNormal, actPos).a > 0.5f)
    {
        return vec3(0);
    }

    int height = int(convSize.y);
    int width = int(convSize.x);

    vec3 avg = vec3(0);

    ivec2 texsz = textureSize(tex, 0);
    float imgWidth = texsz.x;
    float imgHeight = texsz.y;
    vec2 d = vec2(1.f/imgWidth, 1.f/imgHeight) * stepSize;

    vec2 uAcum = vec2(0);
    int convI = 0;

    vec2 pivotDisplacement = pivot * d;
    vec2 initPos = actPos - pivotDisplacement; 

    float box = 1.f/(height * width);

    for(int yy = 0; yy < height; yy++, uAcum.y += d.y){
        uAcum.x = 0;
        for(int xx = 0; xx < width; xx++, uAcum.x += d.x, convI++){
            vec2 nUv = initPos + uAcum;
            
            // si el valor es menor a 0.5 no le hacemos bloom
            

            // if(texture(gNormal, nUv).a < 0.5)
            // {
                avg += texture(tex, nUv).xyz * box;
            // }
        }
    }

    return avg;
}
void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);

    for(int i = 0; i < 1; ++i)
    {
        // calculate distance between light source and current fragment
        float distance = length(LIGHTS[i].Position - FragPos);
        if(distance < LIGHTS[i].Radius)
        {
            // diffuse
            vec3 lightDir = normalize(LIGHTS[i].Position - FragPos);
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * LIGHTS[i].Color;
            // specular
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            vec3 specular = LIGHTS[i].Color * spec * Specular;
            // attenuation
            float attenuation = 1.0 / (LIGHTS[i].Linear * distance + LIGHTS[i].Quadratic * distance * distance);
            diffuse *= attenuation;
            specular *= attenuation;
            lighting += diffuse + specular;
        }
    }    


    // vec3 bloom = blur(gAlbedoSpec, vec2(9,9), vec2(5,5), TexCoords, 1);

    // lighting += bloom;

    FragColor = vec4(lighting, 1.0);

    // FragColor = vec4(vec3(gNormal.a), 1);
}