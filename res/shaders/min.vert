

in vec4 a_position;

uniform mat4 u_worldViewProjectionMatrix;

void main()
{
    vec4 position = a_position;
    gl_Position = u_worldViewProjectionMatrix * position;
}
