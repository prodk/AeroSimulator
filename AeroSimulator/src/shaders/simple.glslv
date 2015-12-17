attribute vec3 aPosition;
attribute vec3 aColor;
uniform mat4 MVP;
varying vec4 vColor;
void main()
{
    gl_Position = MVP * vec4(aPosition, 1.0);
    vColor = vec4(aColor, 1.0f);
}