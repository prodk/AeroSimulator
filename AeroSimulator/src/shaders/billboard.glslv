attribute vec3 aPosition; // Center of the billboard
attribute vec2 aTexCoord;
attribute vec2 aSquad;
uniform mat4 MVP;
uniform vec3 uRight;    // Camera vector right
uniform vec3 uUp;       // Camera vector up
uniform float uWidth;   // Width of the billboard
uniform float uHeight;  // Height of the billboard
varying vec2 vTexCoord;
void main()
{
    vec3 position = aPosition + uRight*aSquad.x * uWidth + uUp*aSquad.y*uHeight;///@todo: change to cross(up, right)
    gl_Position = MVP * vec4(position, 1.0);
    vTexCoord = aTexCoord;
}