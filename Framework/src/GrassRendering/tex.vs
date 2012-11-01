uniform mat4 modelworld;
uniform mat4 worldcamera;
uniform mat4 projection;

void main()
{	  
	// transform vertex to camera coordinates
	gl_Position = projection * worldcamera * modelworld * gl_Vertex;
	
	// get texture coordinate
	gl_TexCoord[0]  = gl_MultiTexCoord0;
}
