uniform mat4 modelworld;
uniform mat4 worldcamera;
uniform mat4 projection;
uniform mat3 worldcameraNormal;
uniform mat3 modelworldNormal;

uniform vec3 lightPosition;
uniform vec3 sunDirection;
uniform float direction;


varying vec3 normal, lightDir;

mat4 setWind(float dir){
	return mat4(1.0, 0.0, 0.0, 0.0,
				dir, 1.0, dir, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
}

void main()
{	 
	vec3 vertex = vec3( worldcamera * setWind(direction/10) * modelworld * gl_Vertex); 

	gl_TexCoord[0]  = gl_MultiTexCoord0;
	// project the point into the camera
	gl_Position = projection * vec4( vertex, 1.0 );	

	normal = normalize( worldcameraNormal * modelworldNormal * gl_Normal );

	vec3 sunDirectionInCamera = vec3(worldcamera*vec4(sunDirection, 1));
	lightDir = normalize(-sunDirectionInCamera);

	gl_FrontColor = gl_Color;
}