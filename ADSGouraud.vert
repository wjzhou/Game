#version 330

in vec4 vertex;
in vec3 normal;
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform vec3 lightPosition;// light in world space
uniform float ns;

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;

smooth out vec4 varyingColor;
void main(void)
{
//    varyingColor=vec4(0.0,0.0,0.0,1.0);
varyingColor = ambientColor;

vec3 vEyeNormal = normalize(normalMatrix * normal);
//vec3 vEyeNormal =normalize(normal);
vec4 vPosition4 = mvMatrix * vertex;
//vec3 vPosition3 =vec3(vPosition4);
vec3 vPosition3 = vPosition4.xyz / vPosition4.w;
// Get vector to light source
vec3 vLightDir = normalize(lightPosition - vPosition3);
// Dot product gives us diffuse intensity
float diff = max(0.0, dot(vEyeNormal, vLightDir));
// Multiply intensity by diffuse color, force alpha to 1.0
varyingColor += diff * diffuseColor;
// Add in ambient light

// Specular Light
vec3 vReflection = normalize(reflect(-vLightDir, vEyeNormal));
float spec = max(0.0, dot(vEyeNormal, vReflection));
if(diff != 0) {
float fSpec = pow(spec, ns);
varyingColor.rgb += vec3(fSpec, fSpec, fSpec);
}
// Don’t forget to transform the geometry!
gl_Position = mvpMatrix * vertex;
}
