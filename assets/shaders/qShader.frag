#version 330 core
in vec2 uv;

out vec4 fragColor;


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;


vec3 blur(sampler2D tex)
{
    // vec3 avg = vec3(0);


    // vec2 uAcum = vec2(0);
    // vec2 d = vec2(1.f/imgWidth, 1.f/imgHeight) * " + std::to_string(d) + ";
    // int convI = 0;

    // vec2 pivotDisplacement = vec2(" + std::to_string(pivotX) + ", " + std::to_string(pivotY) + ") * d;
    // vec2 initPos = actPos - pivotDisplacement; 

    // for(int yy = 0; yy < height; yy++, uAcum.y += d.y){
    //     uAcum.x = 0;
    //     for(int xx = 0; xx < width; xx++, uAcum.x += d.x, convI++){
    //         vec2 nUv = initPos + uAcum;
    //     }
    // }


}


void main()
{    
    vec2 texCoord = uv;

    fragColor = texture(gAlbedoSpec, uv);
}