#version 150

out vec4 fragColor;
in vec4 varyingColor;
in vec2 vVaryingTexCoord;
uniform sampler2D map_kd;
void main( void )
{
    //fragColor = varyingColor;
    //fragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
    fragColor =  texture(map_kd, vVaryingTexCoord);
}
