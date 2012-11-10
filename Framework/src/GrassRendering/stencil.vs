uniform mat4 modelworld;
uniform mat4 worldcamera;
uniform mat4 projection;
uniform mat3 worldcameraNormal;
uniform mat3 modelworldNormal;
uniform vec3 cameraPos;
varying vec3 normal, lightDir;

mat4 getRotationMatrix(vec3 axis, float angle)
{
		float cosa = cos(angle);
		float sina = sin(angle);
		mat4 rotationMatrix;

		rotationMatrix[0][0] = cosa + (1-cosa)*axis.x*axis.x; 
		rotationMatrix[0][1] = (1-cosa)*axis.x*axis.y - axis.z*sina; 
		rotationMatrix[0][2] = (1-cosa)*axis.x*axis.z + axis.y*sina;  
		rotationMatrix[0][3] = 0.f;
		
		rotationMatrix[1][0] = (1-cosa)*axis.x*axis.y + axis.z*sina; 
		rotationMatrix[1][1] = cosa + (1-cosa)*axis.y*axis.y; 
		rotationMatrix[1][2] = (1-cosa)*axis.y*axis.z - axis.x*sina;  
		rotationMatrix[1][3] = 0.f;
		
		rotationMatrix[2][0] = (1-cosa)*axis.x*axis.z - axis.y*sina; 
		rotationMatrix[2][1] = (1-cosa)*axis.y*axis.z + axis.x*sina; 
		rotationMatrix[2][2] = cosa + (1-cosa)*axis.z*axis.z;  
		rotationMatrix[2][3] = 0.f;
		
		rotationMatrix[3][0] = 0.f;
		rotationMatrix[3][1] = 0.f;
		rotationMatrix[3][2] = 0.f;
		rotationMatrix[3][3] = 1.f;

		return rotationMatrix;
}


void main()
{	  

	vec4 verticeInWorld = modelworld * gl_Vertex;
	vec3 normalInWorld = normalize(modelworldNormal * gl_Normal);
	vec3 centerGrassToCamera = normalize(cameraPos - vec3(verticeInWorld));
	float rotationAngle = dot(normalInWorld, centerGrassToCamera);
	mat4 rotationMatrix = getRotationMatrix(cross(normalInWorld, centerGrassToCamera), rotationAngle);

	// transform normal to camera coordinates
	normal = normalize( worldcameraNormal * normalInWorld );
	
	// transform vertex to camera coordinates
	vec3 vertex = vec3( worldcamera * modelworld * rotationMatrix * gl_Vertex);
	//TO MODIFY LATER
	vec3 lightpositionCamera = 1;

	// get texture coordinate
	gl_TexCoord[0]  = gl_MultiTexCoord0;
	// project the point into the camera
	gl_Position = projection * vec4( vertex, 1.0 );
	
	lightDir = normalize(lightpositionCamera-vertex);
}