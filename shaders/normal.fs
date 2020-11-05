#version 330 core
out vec4 FragColor;

in vec3 Normal;  
  

void main()
{
    vec3 c = (normalize(Normal) + 1.0) * (1/2.0);
    FragColor = vec4(c, 1.0);
} 