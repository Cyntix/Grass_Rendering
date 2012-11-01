varying vec3 normal, lightDir, indirectLightDir;

uniform int useTexture;
uniform sampler2D texture;
uniform vec3 diffuseColor;
uniform float specularExp;
uniform vec3 indirectlightcolor;
uniform vec3 lightcolor;
		
void main()
{	
	//Exercise 4.4: Calculate the reflected intensities for the direct sun light (using lightDir and lightcolor) 
	//and indirect light (using indirectLightDir and indirectlightcolor)
		
	vec4 finalcolor = vec4(0.0);
	
	vec3 color = diffuseColor;
	vec3 indcolor = diffuseColor;
    if(useTexture!=0)
    {
        color = texture2D(texture, gl_TexCoord[0].xy).xyz * color;
        indcolor = texture2D(texture, gl_TexCoord[0].xy).xyz * indcolor;
    }
    color = color*lightcolor*max(0.0, dot(normal, lightDir));
    indcolor = indcolor*indirectlightcolor*max(0.0, dot(normal, indirectLightDir));

	//also add a small ambient term
	finalcolor += vec4(color, 1.0) + vec4(indcolor, 1.0) + vec4(color, 1.0)*0.1;	

	gl_FragColor = finalcolor;	
}