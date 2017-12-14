uniform sampler2D texSampler;
uniform bool fhayTextura;
varying vec2 fuv;
varying vec3 fcolor;

void main() { 	
	if(fhayTextura){
	gl_FragColor = texture2D(texSampler, fuv)*vec4(fcolor,1);
	}
	else{
	gl_FragColor = vec4(fcolor,1) ;
	}
}
