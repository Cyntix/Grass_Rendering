varying vec3 normal, lightDir;

uniform bool useTexture;
uniform bool useTransparency;
uniform bool useAlphaToCoverage;
uniform bool useColorVariation;
uniform sampler2D texture;
uniform sampler2D alpha;
uniform vec3 diffuseColor;
uniform float specularExp;
uniform vec3 lightcolor;
		
void main()
{	
    float alphaValue = 1;
    vec4 alphaTex = texture2D(alpha, gl_TexCoord[0].xy);
    if(useTexture && useTransparency){        
        if(!useAlphaToCoverage && alphaTex.x<0.95){
            discard;
        }else{
            alphaValue = texture2D(alpha, gl_TexCoord[0].xy).x;
        }
    }
	vec4 finalcolor = vec4(0.0);	
	vec3 ambiant = diffuseColor;
    if(useTexture)
    {
        ambiant = texture2D(texture, gl_TexCoord[0].xy).xyz * ambiant;
    }
    vec3 diffused = ambiant*lightcolor*max(0.0, dot(normalize(normal), normalize(lightDir)));

	//also add a small ambient term
	finalcolor += vec4(diffused*0.75+ambiant*1.25, alphaValue);	

    if(useColorVariation){
	    gl_FragColor = vec4(pow(finalcolor.r, 1/gl_Color.r), pow(finalcolor.g, 1/gl_Color.g), pow(finalcolor.b, 1/gl_Color.b), finalcolor.a);
    }else{
        gl_FragColor = finalcolor;
    }
}