uniform mat4 modelworld;
uniform mat4 worldcamera;
uniform mat4 projection;
uniform mat3 worldcameraNormal;
uniform mat3 modelworldNormal;
uniform vec3 lightPosition;

varying vec3 normal, lightDir, eyeVec;

void main()
{	  
	vec3 vertex = vec3( worldcamera * modelworld * gl_Vertex );
	
	normal = normalize( worldcameraNormal * modelworldNormal * gl_Normal );
	
	vec3 lightpositionCamera = vec3(worldcamera*vec4(lightPosition, 1.0));

	lightDir = normalize(lightpositionCamera-vertex);
	eyeVec = -vertex;

	// get texture coordinate
	gl_TexCoord[0]  = gl_MultiTexCoord0;
	// project the point into the camera
	gl_Position = projection * vec4( vertex, 1.0 );
	
}
