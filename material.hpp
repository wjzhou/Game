#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "glm/glm.hpp"
#include <tr1/unordered_map>
#include <tr1/memory>
#include <string>
class Material
{
public:
    std::string name;
    glm::vec4 ka;
    glm::vec4 kd;
    glm::vec4 ks;
    float ns;    
    int illum;   //illumination model, I use this to select shade program
    int shader;
    //Material();
    Material(std::string name, int illum, const glm::vec4& ka, const glm::vec4& kd, const glm::vec4& ks, float ns);
    void setupOpenGL();
    static std::tr1::unordered_map<std::string, std::tr1::shared_ptr<Material> >  materials;
    static std::tr1::shared_ptr<Material> defaultMatrial; //default is a keyword.. almost forget that
    static void init();
};

#endif // MATERIAL_HPP
