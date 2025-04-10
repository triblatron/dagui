#version 130

struct Light
{
	vec4 pos;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 rolloff;
	float cutoff;
	float spotExponent;
	int type;
};

const int MAX_LIGHTS = {impl.MAX_LIGHTS};
uniform Light lights[MAX_LIGHTS];
varying vec3 pos;
varying vec4 colour;

void directionalLight(int index, vec3 vertexPos, vec3 eyePos)
{
	vec3 toLight = lights[index].pos.xyz - vertexPos;
	vec3 toEye = eyePos - vertexPos;
	
}

void main()
{
	gl_FragColor = vec4(1,0,0,1);
}


