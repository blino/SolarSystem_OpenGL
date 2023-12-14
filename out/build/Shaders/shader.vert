#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 world;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * world * vec4(pos, 1.0);
	vCol = vec4(clamp(pos, 0.f, 1.f), 1.f);

	TexCoord = tex;

	Normal = mat3(transpose(inverse(world))) *normal;

	FragPos = (world * vec4(pos, 1.0)).xyz;
}