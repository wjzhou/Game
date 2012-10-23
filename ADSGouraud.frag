#version 330

out vec4 fragColor;
in vec4 varyingColor;

void main( void )
{
    fragColor = varyingColor;
    //fragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
}
