#include "node.hpp"

Node::Node(QObject *parent) :
    QObject(parent)
{
}

void Node::draw()
{
    if (geomrtries.size()>0){
        foreach(Geometry* g, geomrtries){
            g->draw();
        }
    }
    foreach(Node* n, nodes){
        n->draw();
    }

}
