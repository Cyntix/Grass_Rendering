varying vec3 normal, lightDir;

uniform int useTexture;
uniform sampler2D texture;
uniform sampler2D alpha;
uniform vec3 diffuseColor;
uniform float specularExp;
uniform vec3 lightcolor;
		
void main()
{	
    vec4 alphaTex = texture2D(alpha, gl_TexCoord[0].xy);
    if(useTexture == 1 && alphaTex.x < 0.95){
        discard;
    }
	vec4 finalcolor = vec4(0.0);
	
	vec3 ambiant = diffuseColor;
    if(useTexture!=0)
    {
        ambiant = texture2D(texture, gl_TexCoord[0].xy).xyz * ambiant;
    }
    vec3 diffused = ambiant*lightcolor*max(0.0, dot(normal, lightDir));

	//also add a small ambient term
	finalcolor += vec4(diffused, 1.0) + vec4(ambiant, 1.0)*0.75;	

	gl_FragColor = finalcolor;
}