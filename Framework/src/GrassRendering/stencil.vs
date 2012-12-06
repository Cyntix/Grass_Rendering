#define M_PI 3.1415926535897932384626433832795

uniform mat4 modelworld;
uniform mat4 worldcamera;
uniform mat4 projection;
uniform mat3 worldcameraNormal;
uniform mat3 modelworldNormal;

uniform vec3 lightPosition;
uniform vec3 sunDirection;

uniform float wind_angle;
uniform float wind_pattern_distance_x;
uniform float wind_pattern_distance_z;
uniform float billboard_scale;


varying vec3 normal, lightDir;

mat4 setWind_x(vec4 vertex){
	float angle_x = wind_angle + (mod(vertex.x, wind_pattern_distance_x)*2*M_PI/wind_pattern_distance_x);
	float dir_x = sin(angle_x) * billboard_scale/10;

	float angle_z = wind_angle + (mod(vertex.z, wind_pattern_distance_z)*2*M_PI/wind_pattern_distance_z);
	float dir_z = sin(angle_z) * billboard_scale/10;

	return mat4(1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				dir_x, 0.0, dir_z, 1.0);
}

void main()
{	 
	vec4 vertexInWorld = modelworld * gl_Vertex;
	vec3 vertexInCamera = vec3( worldcamera * vertexInWorld); 
	if(gl_MultiTexCoord0.y > 0.75){
		vertexInCamera = vec3( worldcamera * setWind_x(vertexInWorld) * vertexInWorld); 
	}

	gl_TexCoord[0]  = gl_MultiTexCoord0;
	// project the point into the camera
	gl_Position = projection * vec4( vertexInCamera, 1.0 );	

	normal = normalize( worldcameraNormal * modelworldNormal * gl_Normal );

	vec3 sunDirectionInCamera = vec3(worldcamera*vec4(sunDirection, 1));
	lightDir = normalize(-sunDirectionInCamera);

	gl_FrontColor = gl_Color;
}