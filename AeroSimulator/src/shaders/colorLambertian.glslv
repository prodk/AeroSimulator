attribute vec3 aPosition;
attribute vec3 aNormal;
uniform mat4 MVP;
uniform mat4 uM;
varying vec3 vEyeNormal;
varying vec3 vPos;
void main()
{
    vEyeNormal = (uM * vec4(aNormal, 0.0)).xyz;
    vPos = (uM * vec4(aPosition, 1.0)).xyz;
    gl_Position = MVP * vec4(aPosition, 1.0);
}