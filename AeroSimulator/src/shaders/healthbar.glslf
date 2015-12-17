uniform vec4 uColor;
uniform float uHealth;  // Health value
varying vec3 vPos;  // Shifted vertex position, model space
void main()
{
    if ((vPos.x > uHealth) || (vPos.y < 0.1) || (vPos.y > 0.9))
       gl_FragColor = uColor;
    else
       gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}