#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct PointLight {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;  
    vec3 color;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

};

in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointlight;
uniform SpotLight spotlight;

vec3 calc_phong(vec3 lightDir, vec3 viewDir, vec3 lightColor, float brightness, Material m){
    // ambient
    vec3 ambient = m.ambient;
    // diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * m.diffuse;

    // specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), m.shininess);
    vec3 specular = spec * m.specular; 
    
    vec3 result = brightness*lightColor*(ambient + diffuse + specular);
    return result;
}


void main()
{
    //-------------------point light---------------//

    vec3 pointlight_dir = normalize(pointlight.position - FragPos);
    vec3 pointlight_color = pointlight.color;
    vec3 viewDir = normalize(viewPos - FragPos);
    // point light attenuation
    float pointlight_distance    = length(pointlight.position - FragPos);
    float pointlight_attenuation = 1.0 / (pointlight.constant + pointlight.linear * pointlight_distance + pointlight.quadratic * (pointlight_distance * pointlight_distance));  
    float pointlight_brightness = clamp(pointlight_attenuation, 0, 1);
    
    vec3 pointlight_result = calc_phong(pointlight_dir, viewDir, pointlight_color, pointlight_brightness, material);

    FragColor = vec4(pointlight_result, 1.0);
} 