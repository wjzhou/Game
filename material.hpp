#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "glm/glm.hpp"
#include <tr1/unordered_map>
#include <tr1/memory>
#include <string>
#include "shader.hpp"

class Material{
public:
    const std::string name;
    virtual void prepareShader()=0;

    static std::tr1::shared_ptr<Material> findMaterialByName(const std::string& name);
    static std::tr1::shared_ptr<Material> defaultMatrial; //default is a keyword.. almost forget that

    static void init();
    static void exit();
    virtual ~Material();
protected:
    Material(const std::string& name);
    GLuint shaderId;
    std::tr1::shared_ptr<Shader> pShader;
    void setShader(std::tr1::shared_ptr<Shader> pShader);
    void setupGeometry(); //this use a global variable called shaderStatus
private:
    static std::tr1::unordered_map<std::string, std::tr1::shared_ptr<Material> > materials;
    GLint locMVP;
    GLint locMV;
    GLint locNM;
    GLint locLight;
};

/*inline const Shader* Material::getShader()
{
    return pShader;
}*/

class ADSMaterial: public Material
{
public:
    glm::vec4 ka;
    glm::vec4 kd;
    glm::vec4 ks;
    float ns;    
    int illum;   //illumination model, I use this to select shade program
    void prepareShader();
    ADSMaterial(const std::string& name, int illum, const glm::vec4& ka, const glm::vec4& kd, const glm::vec4& ks, float ns);
    //ADSMaterial(const std::string& name)
    ADSMaterial(const std::string& aname, int aillum, const glm::vec4& aka,
                       const glm::vec4& akd, const glm::vec4& aks, float ans,
                       std::tr1::shared_ptr<Shader> apShader);
    ~ADSMaterial(){}
    static std::tr1::shared_ptr<Shader> findShaderByIllum(int illum);
    static std::tr1::unordered_map<int, std::tr1::shared_ptr<Shader> > shaders;

private:
    void constructHelper(std::tr1::shared_ptr<Shader> apShader);
    GLint locAmbientColor;
    GLint locColor;
    GLint locSpecularColor;
    GLint locNs;
};

class ADSMaterialWithTexture: public ADSMaterial{
public:
     ADSMaterialWithTexture(const std::string& name, int illum, const glm::vec4& ka, const glm::vec4& kd, const glm::vec4& ks, float ns);
     void setKdMap(const QString& textureFileName);
     void prepareShader();
     static std::tr1::shared_ptr<Shader> textShader;
private:
     GLuint texBuffObj;
};

#endif // MATERIAL_HPP
