#version 430

layout (location = 0) in vec3 position;
//layout (location = 1) in vec2 tex_coord;
//out vec2 tc;
//out vec4 my_color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding=0) uniform sampler2D s;

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

void main(void)
{	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	//tc = tex_coord;
	//my_color = vec4(1.0, 1.0, 1.0, 1.0);
}