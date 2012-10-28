#ifndef OBJPARSER_H
#define OBJPARSER_H
#include <trianglemesh.hpp>
#include <scene/geometry.hpp>
class ObjParser
{
public:
    ObjParser(){};
    //bool parse(std::string filename);
    bool parse(const std::string& fileName, std::vector<Geometry*>& geometries);
private:
    //TriangleMesh& tm;
};

#endif // OBJPARSER_H
