#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "glm/glm.hpp"
#include <tr1/unordered_map>
#include <tr1/memory>
#include <string>
#include "shader.hpp"

class Material{
public:
    Material(const std::string& name, Shader* aShader);
    std::string name;
    const Shader* getShader();
    virtual void prepareShader()=0;
    static std::tr1::unordered_map<std::string, std::tr1::shared_ptr<Material> >  materials;
    static std::tr1::shared_ptr<Material> defaultMatrial; //default is a keyword.. almost forget that
    static void init();
    virtual ~Material();
protected:
    Material(){} //sub class's construct needs to provide pShader to base, but it is too late there
    void constructHelper();
    GLuint shaderId;
    Shader* pShader;
    void setupGeometry(); //this use a global variable called currentMV, currentMVP, and currentNormal
private:
    GLint locMVP;
    GLint locMV;
    GLint locNM;
};

inline const Shader* Material::getShader()
{
    return pShader;
}

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
private:
    GLint locAmbientColor;
    GLint locColor;
    GLint locSpecularColor;
    GLint locNs;
};

#endif // MATERIAL_HPP
