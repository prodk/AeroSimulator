attribute vec3 aPosition; // Center of the billboard
attribute vec2 aSquad;
uniform mat4 MVP;
uniform vec3 uRight;    // Camera vector right
uniform vec3 uUp;       // Camera vector up
uniform float uWidth;   // Width of the billboard
uniform float uHeight;  // Height of the billboard
varying vec3 vPos;  // Shifted vertex position, model space
void main()
{
    vec3 position = aPosition + uRight*aSquad.x * uWidth + uUp*aSquad.y*uHeight;
    vPos = aPosition + vec3(aSquad.x + 0.5, aSquad.y + 0.5f, 0.f);
    gl_Position = MVP * vec4(position, 1.0);
}