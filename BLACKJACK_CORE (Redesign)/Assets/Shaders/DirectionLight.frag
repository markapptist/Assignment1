#version 450 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    vec3 position, direction;
    float cutOff, outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

in vec3 norm;  
in vec3 FragPos;  
in vec2 TexCoord;
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform Material material;
uniform Light light;

uniform sampler2D texture1, texture2;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
  	
    // diffuse 
    vec3 norm = normalize(norm);
    // vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(-light.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoord).rgb;  

	FragColor.rgb += ambient + diffuse + specular;
	vec4 textureColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	FragColor.rgb *= textureColor.rgb;
	FragColor.a = textureColor.a;
        
   // vec3 result = (ambient + diffuse + specular) * objectColor;
   // FragColor = vec4(result, 1.0);


}