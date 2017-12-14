#define MAX_LIGHTS 8
#define MAX_BONES 75

uniform mat4 modelView;
uniform mat4 normalMatrix;

uniform bool lightingEnabled;
uniform bool lightEnabled[MAX_LIGHTS];

uniform vec4 lightPos[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];
uniform float lightAtt[MAX_LIGHTS];

uniform vec3 diffuse;
uniform vec3 ambient;
uniform int shininess;
uniform mat4 MVP;

//P5
uniform bool skinned;
uniform mat4 animMatrices[MAX_BONES];

attribute vec3 vpos;
attribute vec2 vuv;
attribute vec3 vnormal;
varying vec2 fuv;
varying vec3 fcolor;

//P5
attribute vec4 vboneIndices;
attribute vec4 vboneWeights;

void main() {
  
  vec4 vpos4 = vec4(vpos, 1);
  // Animación por huesos
  if (skinned) {
    mat4 boneTransform = mat4(1);
    for (int i = 0; i < 4; ++i) {
      int index = int(vboneIndices[i]);
      if (index > -1) boneTransform += animMatrices[index] * vboneWeights[i];
    }
    vpos4 = boneTransform * vpos4;
  }

	if (lightingEnabled) {
		vec3 combinedDiffuse = ambient;
		vec3 combinedSpecular = vec3(0, 0, 0);
		int it;
		for (it = 0; it < MAX_LIGHTS; ++it) {
			if (lightEnabled[it]) {
        

          vec4 N = normalMatrix*vec4(vnormal, 1);

          float normalizerN = 1 / sqrt((N[0] * N[0] + N[1] * N[1] + N[2] * N[2]));
          N[0] = N[0] * normalizerN;
          N[1] = N[1] * normalizerN;
          N[2] = N[2] * normalizerN;

          vec4 E = modelView*vpos4;
          vec4 L = lightPos[it];
          float att = 1;

          //si puntual 
          if (L[3] == 1) {
            L = L - E;
            att = 1.0 / pow((1 + lightAtt[it] * length(L)),5);
          }

          float normalizerL = 1 / sqrt((L[0] * L[0] + L[1] * L[1] + L[2] * L[2]));
          L[0] = L[0] * normalizerL;
          L[1] = L[1] * normalizerL;
          L[2] = L[2] * normalizerL;

          float NdotL = max(N[0] * L[0] + N[1] * L[1] + N[2] * L[2], 0);
          combinedDiffuse = combinedDiffuse + vec3(NdotL*lightColor[it][0]*att, NdotL*lightColor[it][1]*att, NdotL*lightColor[it][2]*att);
          
          if ((shininess > 0)&&(NdotL>0)) {
            float normalizerE = 1 / sqrt((E[0] * E[0] + E[1] * E[1] + E[2] * E[2]));
            E[0] = E[0] * normalizerE;
            E[1] = E[1] * normalizerE;
            E[2] = E[2] * normalizerE;
            vec4 H= L-E;

            float normalizerH = 1 / sqrt((H[0] * H[0] + H[1] * H[1] + H[2] * H[2]));
            H[0] = H[0] * normalizerH;
            H[1] = H[1] * normalizerH;
            H[2] = H[2] * normalizerH;

            float NdotH = max(N[0] * H[0] + N[1] * H[1] + N[2] * H[2], 0);
            combinedSpecular += vec3(pow(NdotH, shininess)*att, pow(NdotH, shininess)*att, pow(NdotH, shininess)*att);
          }
        
			}
		}

    gl_Position = MVP * vpos4;
		fuv=vuv;
    fcolor = (vec3(diffuse[0] * combinedDiffuse[0], diffuse[1] * combinedDiffuse[1], diffuse[2] * combinedDiffuse[2])+ combinedSpecular);
	}
	else {
		gl_Position = MVP * vpos4;
		fuv = vuv;
		fcolor = diffuse;
	}
}