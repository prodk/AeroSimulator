uniform vec4 uColor;
uniform vec3 uAmbient;
uniform vec3 uDiffuse;
uniform vec3 uSunDir;         // Direction from the fragment to the sun
uniform vec3 uEyePos;         // Camera position
varying vec3 vEyeNormal;      // Fragment normal in the world space
varying vec3 vPos;
void main()
{
    // Diffuse;
    vec3 N = normalize(vEyeNormal);
    vec3 L = normalize(uSunDir);
    float cosD = clamp(dot(N, L), 0, 1);
    // Specular;
    float cosS = 0.0;
    vec3 cameraDir = normalize(uEyePos - vPos);
    vec3 R = reflect(-L, N);
    cosS = clamp(dot(cameraDir, R), 0, 1);
    gl_FragColor = uColor * vec4(uAmbient + uDiffuse * cosD + uDiffuse * pow(cosS, 10), 1);
}