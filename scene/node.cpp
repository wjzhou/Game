#include "common.hpp"
#include "node.hpp"
#include "shader.hpp"
#include "shaderstatus.hpp"
Node::Node(QObject *parent) :
    QObject(parent)
{
}

void Node::draw()
{
    if (geomrtries.size()>0){
        shaderStatus.globalTransform=&globalTransform;
        shaderStatus.shaderId=0; //indicate the transform cache is invalid
        foreach(Geometry* g, geomrtries){
            g->draw();
        }
    }
    foreach(Node* n, nodes){
        n->draw();
    }

}
