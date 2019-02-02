#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 norm;
out vec3 FragPos;
  
uniform mat4 ModelMatrix;
uniform mat4 model, view, projection;

void main() {
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	norm = mat3(view) * mat3(model) * aNormal;

	vec4 viewSpace = view * model * vec4(aPos, 1.0);

    gl_Position = projection * viewSpace;

	FragPos = viewSpace.xyz;
} 