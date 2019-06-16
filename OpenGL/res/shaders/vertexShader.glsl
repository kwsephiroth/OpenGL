#version 430
//in variables
layout (location=0) in vec3 position;

//uniforms


//out variables
out vec4 varyingColor;

void main(void)
{	
	gl_Position = vec4(position,1.0);
	varyingColor = vec4(position,1.0)*0.5 + vec4(0.5, 0.5, 0.5, 0.5);
} 