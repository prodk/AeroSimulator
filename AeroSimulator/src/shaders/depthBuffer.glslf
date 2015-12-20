precision highp float;
varying vec2 vTexCoord;
uniform sampler2D sTexture;

void main()
{
    float depth = 2.0*texture2D(sTexture, vTexCoord).r - 1.0f;
    depth = pow(depth, 60.0);
    gl_FragColor = vec4(vec3(depth), 1.0);
}