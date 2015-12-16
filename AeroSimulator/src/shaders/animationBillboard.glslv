attribute vec3 aPosition;    // Center of the billboard
attribute vec2 aTexCoord;
attribute vec2 aSquad;
uniform mat4 MVP;
uniform vec3 uRight;         // Camera vector right
uniform vec3 uUp;            // Camera vector up
uniform float uWidth;        // Width of the billboard
uniform float uHeight;       // Height of the billboard
uniform vec2 uCurrentFrame;  // Current frame in the sprite
uniform vec2 uFrameSize;     // 1/numOfFrames in given direction
varying vec2 vTexCoord;
void main()
{
    vec3 position = aPosition + uRight*aSquad.x * uWidth + vec3(0.0, 1.0, 0.0)*aSquad.y*uHeight;
    gl_Position = MVP * vec4(position, 1.0);
    vTexCoord = (aTexCoord + uCurrentFrame)*uFrameSize;
}