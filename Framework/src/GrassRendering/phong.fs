varying vec3 normal, lightDir, eyeVec;

uniform int useTexture;
uniform sampler2D texture;
uniform vec3 diffuseColor;
uniform float specularExp;
		
void main()
{	
	vec3 color = ( (useTexture > 0) ? diffuseColor*texture2D(texture, gl_TexCoord[0].xy).xyz : diffuseColor );
	

	vec4 final_color = vec4(0.0);				
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	float lambertTerm = dot(N, L);
	if(lambertTerm > 0.0)
	{
	
		final_color += vec4(color, 1.0) * lambertTerm;	
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);
		float specular = pow(max(dot(R, E), 0.0), specularExp);
		final_color += vec4(color, 1.0)*specular;	
		
	}
	
	float coef = smoothstep(1.0,0.2,max(lambertTerm,0.0));
	final_color += coef * vec4(color, 1.0);
	
	//ambient
	final_color += vec4(color,1.0)*0.1;

	gl_FragColor = final_color;	
	
	
}