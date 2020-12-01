#version 330 core
out vec4 FragColor;

struct Material {
    vec3 color;
    float ambient;
    float diffuse;
    float specular;    
    float shininess;
}; 

struct DirLight {
    vec3 direction;
    vec3 color;
    float brightness;
};


in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirlight;

uniform sampler2D texture_ambient1;
uniform sampler2D texture_diffuse1; 
uniform sampler2D texture_specular1;  
uniform float C; // toon factor

float rectify(float edge, float C)
{
    if(edge < C) return 0;
    else return edge;
}

vec3 calc_phong_toon(vec3 lightDir, vec3 viewDir, vec3 lightColor, float brightness, Material m, float C)
{
    // ambient
    float ambient = m.ambient;
    // diffuse 
    vec3 norm = normalize(Normal);
    float diff = rectify(max(dot(norm, lightDir), 0.0), C);
    float diffuse = diff * m.diffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(rectify(max(dot(viewDir, reflectDir), 0.0), C), m.shininess);
    float specular = spec * m.specular; 
    
    // discretize light intensity
    float intensity = ambient + diffuse + specular;
    if (intensity > 0.95)
    {
        intensity = 1;
    }
    else if (intensity > 0.5)
    {
        intensity = 0.7;
    }
    else if (intensity > 0.05)
    {
        intensity = 0.35;
    }
    else
    {
        intensity = 0.05;
    }

    vec3 result = brightness*intensity*lightColor*m.color;
    return result;
}


void main()
{
    //-------------------direct light---------------//

    vec3 lightdir = normalize(-dirlight.direction);
    vec3 lightcolor = dirlight.color;
    vec3 viewDir = normalize(viewPos - FragPos);
    Material m = material;
    m.color = vec3(texture(texture_diffuse1, TexCoords));
    vec3 pointlight_result = calc_phong_toon(lightdir, viewDir, lightcolor, 1, m, C);

    FragColor = vec4(pointlight_result, 1.0);
} 