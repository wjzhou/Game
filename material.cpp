#include "common.hpp"
#include "material.hpp"
#include "shader.hpp"



Material::Material(std::string aname, int aillum, const glm::vec4& aka,
                   const glm::vec4& akd, const glm::vec4& aks, float ans)
    :name(aname),ka(aka),kd(akd),ks(aks),ns(ans),illum(aillum)
{
    illum=2;
    shader=Shader::findShaderByIllum(illum);
    materials.insert(std::make_pair(name, std::tr1::shared_ptr<Material>(this)));
}

void Material::setupOpenGL()
{
    glUseProgram(shader);
    checkGLError("Material,1");
    GLint locAmbientColor = glGetUniformLocation(shader, "ambientColor");
    glUniform4fv(locAmbientColor, 1, &ka[0]);
    checkGLError("Material,2");
    GLint locColor = glGetUniformLocation(shader, "diffuseColor");
    glUniform4fv(locColor, 1, &kd[0]);
    checkGLError("Material,3");
    GLint locSpecularColor = glGetUniformLocation(shader, "specularColor");
    glUniform4fv(locSpecularColor, 1, &ks[0]);

    GLint locNs = glGetUniformLocation(shader, "ns");
    glUniform1f(locNs, ns);
}

std::tr1::unordered_map<std::string, std::tr1::shared_ptr<Material> >
 Material::materials;

void Material::init(){
      defaultMatrial.reset(new Material("default", 2,
                     glm::vec4(0.0f, 0.0f, 0.2f, 0.0f),
                     glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
                     glm::vec4(0.0f, 0.0f, 0.5f, 0.0f), 20));
}

std::tr1::shared_ptr<Material>  Material::defaultMatrial;
