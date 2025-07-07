#version 330 core

out vec4 fragColor;

float near = 0.1;
float far = 100.0;

void main()
{
    // Convert from [0,1] to NDC range [-1,1]
    float depth = gl_FragCoord.z * 2.0 - 1.0;
    
    // Convert from projective depth to linear depth in the range [near, far]
    depth = (2.0 * near * far) / (far + near - depth * (far - near));
    
    // Convert from [near, far] to [0,1]
    depth = (depth - near) / (far - near);

    fragColor = vec4(vec3(depth), 1.0);
}
