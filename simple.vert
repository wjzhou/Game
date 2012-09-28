#version 330 core

in vec4 vertex;

void main( void )
{
    mat4 m=mat4(     0.2, 0.0, 0.0, 0.0,
                0.0, 0.2, 0.0, 0.0,
                0.0, 0.0, 0.2, 0.0,
                0.0, 0.0, 0.0, 1.0);
    gl_Position = m*vertex;
    //gl_Position = vertex;
}
