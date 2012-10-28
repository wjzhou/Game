#ifndef NODE_HPP
#define NODE_HPP
#include <vector>
#include <QObject>
#include <scene/geometry.hpp>

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(QObject *parent = 0);
    
    std::vector<Node*> nodes;
    std::vector<Geometry*> geomrtries;
    void draw();

signals:
    
public slots:
    
};

#endif // NODE_HPP
