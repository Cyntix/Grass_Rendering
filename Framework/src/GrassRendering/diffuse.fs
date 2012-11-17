varying vec3 normal, eyeVec, lightDir;

uniform int useTexture;
uniform sampler2D texture;
uniform sampler2D normal_map;
uniform vec3 diffuseColor;

uniform vec3 cameraPos;
uniform vec3 lightcolor;
		
void main()
{	
	//Exercise 4.4: Calculate the reflected intensities for the direct sun light (using lightDir and lightcolor) 
	//and indirect light (using indirectLightDir and indirectlightcolor)
		
	vec4 finalcolor = vec4(0.0, 0.0, 0.0, 0.0);
	
	vec3 color = diffuseColor;
    vec3 normalColor = vec3(0, 0, 0);
    if(useTexture!=0)
    {
        color = texture2D(texture, gl_TexCoord[0].xy).xyz * color;
        normalColor = texture2D(normal_map, gl_TexCoord[0].xy).xyz;
    }
    color = color*lightcolor*max(0.0, dot(normal, lightDir));

    //specular
    vec3 vVec = normalize(eyeVec);
    double specular = pow(clamp(dot(reflect(-lightDir, normalColor), vVec), 0.0, 1.0), 10);
	//also add a small ambient term
	finalcolor += vec4(color, 1.0) + vec4(specular, specular, specular, 1.0)*0.1 + vec4(diffuseColor, 1.0)*0.1;	

	gl_FragColor = finalcolor;
}