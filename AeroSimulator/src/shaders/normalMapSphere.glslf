vec3 uAmbient = vec3(0.3f, 0.3f, 0.3f);
vec3 uDiffuse = vec3(0.8f, 0.8f, 0.8f);
uniform vec3 uSunDir;        // Direction from the fragment to the sun (world space)
uniform vec3 uEyePos;         // Camera position world
uniform sampler2D sTexture;
uniform sampler2D sNormalMap;
uniform sampler2D sAnimation;
uniform mat4 uM;
varying vec2 vTexCoord;
varying vec2 vTexCoordAnim;
varying vec3 vEyeNormal;     // Fragment normal in the world space
varying vec3 vPos;
varying mat3 mTBN;
void main()
{
    // Diffuse;
    vec4 Nmodel = uM * vec4(normalize(texture2D( sNormalMap, vTexCoord ).rgb*2.0 - 1.0), 0.0);
    vec3 N = Nmodel.xyz;
    vec3 L = mTBN * normalize(uSunDir);
    float cosD = clamp(dot(N, L), 0, 1);
    // Specular
    float cosS = 0.0;
    if (cosD > 0.0) 
	{
       vec3 cameraDir = mTBN * normalize(uEyePos - vPos);
       vec3 R = reflect(-L, N);
       cosS = clamp(dot(cameraDir, R), 0, 1);
    }
    gl_FragColor = (texture2D(sTexture, vTexCoord) + texture2D(sAnimation, vTexCoordAnim)) * vec4(uAmbient + uDiffuse * cosD + uDiffuse * pow(cosS, 10), 1);
//    gl_FragColor = vec4(vTexCoord.x, vTexCoord.y, 0.0, 1.0);// * vec4(uAmbient + uDiffuse * cosD + uDiffuse * pow(cosS, 10), 1);
}