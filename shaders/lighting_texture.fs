#version 330 core
out vec4 FragColor;

struct PointLight {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform PointLight pointlight;

uniform sampler2D texture_ambient1;
uniform sampler2D texture_diffuse1; 
uniform sampler2D texture_specular1;  

vec3 calc_phong(vec3 lightDir, vec3 viewDir, vec3 lightColor, float brightness, vec3 ambient, vec3 diffuse, vec3 specular, float shininess){
    // ambient

    // diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    
    vec3 result = brightness*lightColor*(ambient + diff * diffuse + spec * specular);
    return result;
}

void main()
{    
    vec3 pointlight_dir = normalize(pointlight.position - FragPos);
    vec3 pointlight_color = pointlight.color;
    vec3 viewDir = normalize(viewPos - FragPos);
    // point light attenuation
    float pointlight_distance    = length(pointlight.position - FragPos);
    float pointlight_attenuation = 1.0 / (pointlight.constant + pointlight.linear * pointlight_distance + pointlight.quadratic * (pointlight_distance * pointlight_distance));  
    float pointlight_brightness = clamp(pointlight_attenuation, 0, 1);

    vec3 pointlight_result = calc_phong(pointlight_dir, viewDir, pointlight_color, pointlight_brightness, vec3(0), vec3(texture(texture_diffuse1, TexCoords)), vec3(0), 32);

    FragColor = vec4(pointlight_result, texture(texture_diffuse1, TexCoords)[3]);
}