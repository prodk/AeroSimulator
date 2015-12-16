attribute vec3 aPosition;
attribute vec2 aTexCoord;
uniform mat4 MVP;
varying vec2 vTexCoord;
void main()
{
    gl_Position = MVP * vec4(aPosition, 1.0);
    vTexCoord = aTexCoord;
}