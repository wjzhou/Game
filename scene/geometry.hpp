#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include "common.hpp"
#include <tr1/memory>
#include <QObject>
#include "material.hpp"
#include "trianglemesh.hpp"

class Geometry : public QObject
{
    Q_OBJECT
public:
    explicit Geometry(QObject *parent = 0);
    Geometry();
    Geometry(std::tr1::shared_ptr<TriangleMesh> apTriangleMesh,
             std::tr1::shared_ptr<Material> apMaterial,
             unsigned int aindexStart, unsigned int alength)
        :pMaterial(apMaterial),pTriangleMesh(apTriangleMesh),
          indexStart(aindexStart),length(alength){}

    std::tr1::shared_ptr<Material> pMaterial;
    std::tr1::shared_ptr<TriangleMesh> pTriangleMesh;
    unsigned int indexStart;
    unsigned int length;
    void draw();
signals:
    
public slots:
    
};

#endif // GEOMETRY_HPP
