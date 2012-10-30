#include "common.hpp"
#include "material.hpp"
#include "shader.hpp"
#include "shaderstatus.hpp"


Material::Material(const std::string& aname, Shader* aShader)
    :name(aname),pShader(aShader)
{
    constructHelper();
}

Material::~Material(){
    Material::materials.erase(name);
}

void Material::constructHelper(){
    shaderId=pShader->shaderId;
    checkGLError("Material,1");
    locMVP = glGetUniformLocation(shaderId, "mvpMatrix");
    locMV = glGetUniformLocation(shaderId, "mvMatrix");
    locNM = glGetUniformLocation(shaderId, "normalMatrix");
    checkGLError("Material,2");
    Material::materials.insert(std::make_pair(name, std::tr1::shared_ptr<Material>(this)));
}

void Material::setupGeometry()
{
    checkGLError("Material,1");
    Transform* gTransform=shaderStatus.globalTransform;
    glm::mat4 mvMatrix=(*shaderStatus.view)*gTransform->HMatrix();
    glm::mat3 normalMatrix=glm::transpose(glm::inverse(glm::mat3(mvMatrix)));
    glm::mat4 mvpMatrix=(*shaderStatus.perspective)*mvMatrix;
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvpMatrix[0][0]);
    glUniformMatrix4fv(locMV, 1, GL_FALSE, &mvMatrix[0][0]);
    glUniformMatrix3fv(locNM, 1, GL_FALSE, &normalMatrix[0][0]);
    checkGLError("Material,2");
}

std::tr1::unordered_map<std::string, std::tr1::shared_ptr<Material> >
 Material::materials;

void Material::init(){
      new Shader(2, "/home/wujun/workspace/game/opengl/ADSGouraud.vert",
                 "/home/wujun/workspace/game/opengl/ADSGouraud.frag");
      defaultMatrial.reset(new ADSMaterial("default", 2,
                     glm::vec4(0.0f, 0.0f, 0.2f, 0.0f),
                     glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
                     glm::vec4(0.0f, 0.0f, 0.5f, 0.0f), 20));
}

std::tr1::shared_ptr<Material>  Material::defaultMatrial;

ADSMaterial::ADSMaterial(const std::string& aname, int aillum, const glm::vec4& aka,
                   const glm::vec4& akd, const glm::vec4& aks, float ans)
    :ka(aka),kd(akd),ks(aks),ns(ans),illum(aillum)
{
    name=aname;
    pShader=Shader::findShaderByIllum(illum);
    constructHelper();
    checkGLError("ADSMaterial,1");
    locAmbientColor = glGetUniformLocation(shaderId, "ambientColor");
    locColor = glGetUniformLocation(shaderId, "diffuseColor");
    locSpecularColor = glGetUniformLocation(shaderId, "specularColor");
    locNs = glGetUniformLocation(shaderId, "ns");
    checkGLError("ADSMaterial,2");
}

void ADSMaterial::prepareShader()
{
    GLuint shaderId=pShader->shaderId;
    if (shaderId != shaderStatus.shaderId){ //if we change shader, we
        shaderStatus.shaderId=shaderId;      //need to reset geometry uniform
        glUseProgram(shaderId);
        setupGeometry();
    }
    if (this!=shaderStatus.material){
        checkGLError("ADSMaterial,3");
        glUniform4fv(locAmbientColor, 1, &ka[0]);
        glUniform4fv(locColor, 1, &kd[0]);
        glUniform4fv(locSpecularColor, 1, &ks[0]);
        glUniform1f(locNs, ns);
        checkGLError("ADSMaterial,4");
        shaderStatus.material=this;
    }
}

ShaderStatus shaderStatus;
