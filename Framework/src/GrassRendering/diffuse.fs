varying vec3 normal, eyeVec, lightDir;

uniform int useTexture;
uniform sampler2D texture;
uniform sampler2D normal_map;
uniform vec3 diffuseColor;

uniform vec3 cameraPos;
uniform vec3 lightcolor;
		
void main()
{			
    vec3 newLightDir = normalize(lightDir);
    
	vec4 finalcolor = vec4(0.0, 0.0, 0.0, 0.0);
	vec3 finalNormal = normalize(normal);
	
	vec3 color = diffuseColor;
    vec3 normalColor = vec3(0, 0, 0);
    if(useTexture!=0)
    {
        color = texture2D(texture, gl_TexCoord[0].xy).xyz * color;
        normalColor = texture2D(normal_map, gl_TexCoord[0].xy).xyz;
		finalNormal = normalize(vec3(normal.x + normalColor.x, normal.y + normalColor.y, normal.z + normalColor.z));
    }
    color = color*lightcolor*max(0.0, dot(finalNormal, newLightDir));

    //specular
    vec3 vVec = normalize(eyeVec);
    double specular = pow(clamp(dot(reflect(-newLightDir, normalColor), vVec), 0.0, 1.0), 10);

	finalcolor += vec4(color + vec3(specular, specular, specular)*0.1 + diffuseColor*0.1, 1.0);	

	gl_FragColor = finalcolor;
}