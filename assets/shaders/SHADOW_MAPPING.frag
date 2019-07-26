#version 330 core
#define sdot(a,b) min( max(dot(a,b), 0) , 1)
#define v4(a) vec4(a,1)
#define vec3 vec4
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2


uniform float near, far; // vherever you are

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

out vec3 fvec3;

void main()
{

   float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    fvec3 = vec4(vec3(depth), 1.0);
}