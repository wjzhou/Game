#ifndef OBJPARSER_H
#define OBJPARSER_H
#include <trianglemesh.hpp>
class TriangleMesh;
class ObjParser
{
public:
    ObjParser(TriangleMesh& tm);
    bool parse(std::string filename);
private:
    TriangleMesh& tm;
};

#endif // OBJPARSER_H
