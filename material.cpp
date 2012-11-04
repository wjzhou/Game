#include "common.hpp"
#include "material.hpp"
#include "shader.hpp"
#include "shaderstatus.hpp"
#include "glwidget.h"

Material::Material(const std::string& aname)
    :name(aname)
{
    Material::materials.insert(std::make_pair(name, std::tr1::shared_ptr<Material>(this)));
}

Material::~Material(){
    Material::materials.erase(name);
}

void Material::setShader(std::tr1::shared_ptr<Shader> apShader){
    pShader=apShader;
    shaderId=pShader->getShaderId();
    checkGLError("Material,1");
    locMVP = glGetUniformLocation(shaderId, "mvpMatrix");
    locMV = glGetUniformLocation(shaderId, "mvMatrix");
    locNM = glGetUniformLocation(shaderId, "normalMatrix");
    locLight = glGetUniformLocation(shaderId, "lightPosition");
    checkGLError("Material,2");
}

void Material::setupGeometry()
{
    checkGLError("Material,3");
    Transform* gTransform=shaderStatus.globalTransform;
    glm::mat4 mvMatrix=(shaderStatus.camera->getViewMatrix())*gTransform->HMatrix();
    glm::mat3 normalMatrix=glm::transpose(glm::inverse(glm::mat3(mvMatrix)));
    glm::mat4 mvpMatrix=(shaderStatus.camera->getPerspectiveMatrix())*mvMatrix;
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvpMatrix[0][0]);
    glUniformMatrix4fv(locMV, 1, GL_FALSE, &mvMatrix[0][0]);
    glUniformMatrix3fv(locNM, 1, GL_FALSE, &normalMatrix[0][0]);
    glUniform3fv(locLight, 1, &(*shaderStatus.lightPosition)[0]);
    checkGLError("Material,4");
}

std::tr1::unordered_map<std::string, std::tr1::shared_ptr<Material> >
 Material::materials;
std::tr1::shared_ptr<Material>  Material::defaultMatrial;

std::tr1::shared_ptr<Material> Material::findMaterialByName(const std::string& name)
{
    std::tr1::unordered_map<std::string, std::tr1::shared_ptr<Material> >::const_iterator it=materials.find(name);
    if (it==materials.end()){
        qWarning("Unknown material [%s] fall back to default material", name.c_str());
        return defaultMatrial;
    }
    return it->second;
}

void Material::init(){
    std::tr1::shared_ptr<Shader> pShader(
                new Shader("./ADSGouraud.vert",
                           "./ADSGouraud.frag"));
    ADSMaterial::shaders.insert(std::make_pair(2, pShader));
    new ADSMaterial("default", 2,
                     glm::vec4(0.0f, 0.0f, 0.2f, 0.0f),
                     glm::vec4(0.0f, 0.0f, 0.6f, 0.0f),
                     glm::vec4(0.0f, 0.0f, 0.5f, 0.0f), 20);
    defaultMatrial=Material::findMaterialByName("default");
    ADSMaterialWithTexture::textShader.reset(
                new Shader("./ADSGouraudTexture.vert",
                           "./ADSGouraudTexture.frag"));
}

void Material::exit(){
    //because the destruct order is undefined for class member, we need to call
    //this before system destroy shader
    Material::materials.clear();
}


ADSMaterial::ADSMaterial(const std::string& aname, int aillum, const glm::vec4& aka,
                   const glm::vec4& akd, const glm::vec4& aks, float ans)
    :Material(aname),ka(aka),kd(akd),ks(aks),ns(ans),illum(aillum)
{
    constructHelper(ADSMaterial::findShaderByIllum(illum));
}

ADSMaterial::ADSMaterial(const std::string& aname, int aillum, const glm::vec4& aka,
                   const glm::vec4& akd, const glm::vec4& aks, float ans,
                   std::tr1::shared_ptr<Shader> apShader)
    :Material(aname),ka(aka),kd(akd),ks(aks),ns(ans),illum(aillum)
{
    constructHelper(apShader);
}

void
ADSMaterial::constructHelper(std::tr1::shared_ptr<Shader> apShader){
    setShader(apShader);
    checkGLError("ADSMaterial,1");
    locAmbientColor = glGetUniformLocation(shaderId, "ambientColor");
    locColor = glGetUniformLocation(shaderId, "diffuseColor");
    locSpecularColor = glGetUniformLocation(shaderId, "specularColor");
    locNs = glGetUniformLocation(shaderId, "ns");
    checkGLError("ADSMaterial,2");
}


ShaderStatus shaderStatus;

void ADSMaterial::prepareShader()
{
    //shaderId=pShader->shaderId;
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

std::tr1::shared_ptr<Shader>
ADSMaterial::findShaderByIllum(int illum)
{
    std::tr1::unordered_map<int,std::tr1::shared_ptr<Shader> >::const_iterator it=shaders.find(illum);
    if (it==shaders.end()){
        qWarning("Unknown illum model fall back to ADS");
        return findShaderByIllum(2);
    }
    return it->second;
}

std::tr1::unordered_map<int, std::tr1::shared_ptr<Shader> > ADSMaterial::shaders;


ADSMaterialWithTexture::ADSMaterialWithTexture(const std::string &name, int illum, const glm::vec4 &ka, const glm::vec4 &kd, const glm::vec4 &ks, float ns)
    :ADSMaterial(name,illum, ka, kd, ks, ns, ADSMaterialWithTexture::textShader)
{
    checkGLError("ADSTextMaterial,1");
    GLuint locMap_Ka=glGetUniformLocation(shaderId, "map_kd");
    checkGLError("ADSTextMaterial,2");
    glUseProgram(shaderId);
    glUniform1i(locMap_Ka, 0);
    glBindAttribLocation(shaderId,2,"texCoord");
    checkGLError("ADSTextMaterial,3");

}

void ADSMaterialWithTexture::setKdMap(const QString& textureFileName){
    //texBuffObj=currentContext()->bindTexture(textureFileName);
    texBuffObj=glWidget->bindTexture(textureFileName);
}

void ADSMaterialWithTexture::prepareShader(){
    ADSMaterial::prepareShader();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texBuffObj);
}

std::tr1::shared_ptr<Shader> ADSMaterialWithTexture::textShader;
