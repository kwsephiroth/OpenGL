#version 430

//in vec2 tc;
//in vec4 my_color;
out vec4 color;

struct PositionalLight
{	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	vec3 position;
};

struct Material
{	vec4 ambient;  
	vec4 diffuse;  
	vec4 specular;  
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding=0) uniform sampler2D s;

void main(void)
{
	//color = texture(s,tc);
	//color = my_color;

	//color = /*(globalAmbient +*/ (material.diffuse + material.specular);
	color = material.diffuse;
}
