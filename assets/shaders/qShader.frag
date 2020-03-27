#version 330 core
in vec2 uv;

out vec4 fragColor;

uniform float stepSize = 1.f;

uniform sampler2D img;



vec3 blur(sampler2D tex, vec2 convSize, vec2 pivot, vec2 actPos, float stepSize)
{
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
            avg += texture(tex, nUv).xyz * box;
        }
    }

    return avg;
}


void main()
{    
    vec2 texCoord = uv;

    fragColor = texture(img, uv);
    // fragColor = vec4(blur(img, vec2(7,7), vec2 (3,3), uv, 1), 1);
}