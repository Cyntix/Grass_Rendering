varying vec3 normal, lightDir;

uniform int useTexture;
uniform int useAlphaToCoverage;
uniform sampler2D texture;
uniform sampler2D alpha;
uniform vec3 diffuseColor;
uniform float specularExp;
uniform vec3 lightcolor;
		
void main()
{	
    float alphaValue = 1;
    vec4 alphaTex = texture2D(alpha, gl_TexCoord[0].xy);
    if(useTexture == 1){        
        if(useAlphaToCoverage != 1 && alphaTex.x<0.95){
            discard;
        }else{
            alphaValue = texture2D(alpha, gl_TexCoord[0].xy).x;
        }
    }
	vec4 finalcolor = vec4(0.0);
	
	vec3 ambiant = diffuseColor;
    if(useTexture!=0)
    {
        ambiant = texture2D(texture, gl_TexCoord[0].xy).xyz * ambiant;
    }
    vec3 diffused = ambiant*lightcolor*max(0.0, dot(normal, lightDir));

	//also add a small ambient term
	finalcolor += vec4(diffused+ambiant*0.75, alphaValue);	

	gl_FragColor = finalcolor;
}