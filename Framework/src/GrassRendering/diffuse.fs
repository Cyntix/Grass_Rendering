varying vec3 normal, lightDir, eyeVec;

uniform int useTexture;
uniform sampler2D texture;
uniform sampler2D normal_map;
uniform vec3 diffuseColor;

uniform vec3 cameraPos;
uniform vec3 lightcolor;
		
void main()
{			
    vec3 finalLightDir = normalize(lightDir);
    
	vec4 finalcolor = vec4(0.0, 0.0, 0.0, 0.0);
	vec3 finalNormal = normalize(normal);
	
    vec3 ambiant = diffuseColor;
    vec3 normalColor = vec3(0, 0, 0);
    if(useTexture!=0)
    {
        ambiant = texture2D(texture, gl_TexCoord[0].xy*5).rgb * ambiant;
        normalColor = texture2D(normal_map, gl_TexCoord[0].xy*5).rgb;
		finalNormal = normalize(finalNormal + normalColor);
    }
    vec3 finalDiffuseColor = ambiant*lightcolor*max(0.0, dot(finalNormal, finalLightDir))*0.25;
    
    vec3 vVec = normalize(eyeVec);
    double specular = pow(clamp(dot(reflect(-finalLightDir, normalColor), vVec), 0.0, 100), 1);

	finalcolor += vec4(finalDiffuseColor + specular*0.1 + ambiant*0.1, 1.0);	

	gl_FragColor = finalcolor;
}