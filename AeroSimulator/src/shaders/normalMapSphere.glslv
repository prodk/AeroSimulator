float pi = 3.14159265f;
float r = 1.0f;
attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec3 aTangent;
uniform mat4 MVP;
uniform mat4 uM;
uniform vec2 uCurrentFrame;  // Current frame in the sprite
uniform vec2 uFrameSize;   // 1/numOfFrames in given direction
varying vec2 vTexCoord;
varying vec2 vTexCoordAnim;
varying vec3 vEyeNormal;  ///@todo: remove from varyings
varying vec3 vPos;
varying mat3 mTBN;
void main()
{
    vec3 Bworld = (uM * vec4(cross(aNormal, aTangent), 0.0)).xyz;
    vec3 Tworld = (uM * vec4(aTangent, 0.0)).xyz;
    vEyeNormal = (uM * vec4(aNormal, 0.0)).xyz;
    mTBN = transpose(mat3(Tworld, Bworld, vEyeNormal));
    vPos = (uM * vec4(aPosition, 1.0)).xyz;
    vTexCoord.y = acos(aPosition.y/r)/(pi);
    vTexCoord.x = (atan(aPosition.z, aPosition.x) + pi)/(2.0f*pi);
    vTexCoordAnim = (vTexCoord + uCurrentFrame)*uFrameSize;
    gl_Position = MVP * vec4(aPosition, 1.0);
}