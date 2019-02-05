#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
    //vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 world_pos;  
in vec3 world_normal;  
in vec2 texcoord;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform bool lightDefault;

//rim lighting
//layout(location = 0) out vec4 out_color;

uniform vec3 light_position;
uniform vec3 eye_position;

uniform sampler2D texture1;

//can pass them as uniforms
const vec3 DiffuseLight = vec3(0.05, 0.05, 0.0);
const vec3 RimColor  = vec3(0.3, 0.3, 0.3);

//higher gamma to get a darker image
const float gamma = 1.0/0.6;

uniform bool rimLighting;


void main()
{
	if (lightDefault == true)
	{
	    // ambient
	    vec3 ambient = light.ambient * texture(material.diffuse, texcoord).rgb;
	  	
	    // diffuse 
	    vec3 norm = normalize(world_normal);
	    // vec3 lightDir = normalize(light.position - FragPos);
	    vec3 lightDir = normalize(-light.direction);  
	    float diff = max(dot(norm, lightDir), 0.0);
	    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texcoord).rgb;  
	    
	    // specular
	    vec3 viewDir = normalize(viewPos - world_pos);
	    vec3 reflectDir = reflect(-lightDir, norm);  
	    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	    vec3 specular = light.specular * spec * texture(material.specular, texcoord).rgb;  
	        
	    vec3 result = ambient + diffuse + specular;
	    FragColor = vec4(result, 1.0);
	}

	if (rimLighting == true)
	{
		vec3 tex1 = texture(texture1, texcoord).rgb;
	
		//get light an view directions
		vec3 L = normalize( light_position - world_pos);
		vec3 V = normalize( eye_position - world_pos);
	
		//diffuse lighting
		vec3 diffuse = DiffuseLight * max(0, dot(L,world_normal));
		
		//rim lighting
		float rim = 1 - max(dot(V, world_normal), 0.0);
		rim = smoothstep(0.6, 1.0, rim);
		vec3 finalRim = RimColor * vec3(rim, rim, rim);
	
		//get all lights and texture
		vec3 finalColor = finalRim + diffuse + tex1;
		
		vec3 finalColorGamma = vec3(pow(finalColor.r, gamma),
									pow(finalColor.g, gamma),
									pow(finalColor.b, gamma));
	
		FragColor = vec4(finalColorGamma, 1);
	}
} 