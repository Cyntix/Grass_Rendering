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
    if(useTexture == 1 && alphaTex.x <0.5){
        discard;
    }
	vec4 finalcolor = vec4(0.0);
	
	vec3 color = diffuseColor;
    if(useTexture!=0)
    {
        color = texture2D(texture, gl_TexCoord[0].xy).xyz * color;
    }
    color = color*lightcolor*max(0.0, dot(normal, lightDir));

	//also add a small ambient term
	finalcolor += vec4(color, 1.0) + vec4(color, 1.0)*0.1;	

	gl_FragColor = finalcolor;
}