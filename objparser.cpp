#include "objparser.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <tr1/array>
#include "common.hpp"
#include <QtDebug>
#include <tr1/memory>
#include "material.hpp"
#include "scene/geometry.hpp"

/*ObjParser::ObjParser(TriangleMesh& atm)
    :tm(atm)
{

}*/
//ObjParser::ObjParser

struct CombinedIndex{
    unsigned int v;
    unsigned int vt;
    unsigned int vn;
    unsigned int index;
    CombinedIndex* next;
    bool operator==(CombinedIndex& b){
        return v==b.v && vt==b.vt &&vn==b.vn;
    }
    CombinedIndex(){v=vt=vn=0;next=NULL;}
    CombinedIndex(const CombinedIndex& a){v=a.v; vt=a.vt; vn=a.vn; next=NULL;}
    CombinedIndex& operator= (const CombinedIndex& a){v=a.v; vt=a.vt; vn=a.vn; next=NULL; return *this;}
};

static
int findIndex(std::vector<CombinedIndex *>& combineIndices,
                     CombinedIndex& combineIndex, unsigned int& currIndex)
{
    int v=combineIndex.v;
    CombinedIndex* cv = combineIndices[v];
    CombinedIndex** pv = &combineIndices[v];

    while(cv){
        if (*cv == combineIndex)
            return cv->index;
        pv=&cv->next;
        cv=cv->next;
    }
    cv=new CombinedIndex(combineIndex);
    *pv=cv;
    cv->index=currIndex++;
    return cv->index;
}

bool ObjParser::parse(const std::string& fileName,
                      std::vector<Geometry*>& geometries)
{
    FILE* f;
    char* buff=NULL;
    size_t buffLen=0;
    size_t linelen;


    std::vector<glm::vec3> tVertices;
    std::vector<glm::vec3> tNormals;
    std::vector<glm::vec3> tTexCoords;
    std::vector<glm::uint> tIndices;
    std::vector<CombinedIndex *> combineIndices;
    CombinedIndex combineIndex;
    unsigned int currIndex=0;

    int components;
    int firstIndex, prevIndex;
    int index;

    char* saved_ptr; //used by strtok_r
    char* token;

    int indexStart=0;

    bool hasNormal=false;
    bool hasTexCoord=false;
    glm::vec3 zero(0.0f, 0.0f, 0.0f);
    glm::vec3 p=zero;
    tVertices.push_back(zero); //obj file index start from 1
    tNormals.push_back(zero);
    tTexCoords.push_back(zero);

    if ((f=fopen(fileName.c_str(),"r"))==NULL){
        qDebug() << "can not open file " << fileName.c_str()
                 << ": no such file or directory";
      qDebug() << "Testing";
      return false;
    }

    std::tr1::shared_ptr<TriangleMesh> ptm(new TriangleMesh());
    std::tr1::shared_ptr<Material> pm=Material::defaultMatrial;

    while((linelen=getline(&buff, &buffLen, f))!=-1){ //use c language for
                                                      //better performance
        if (buff[0]=='v'){
            p=glm::vec3(0.0f, 0.0f, 0.0f);
            components=sscanf(buff+2, "%f %f %f", &p[0], &p[1], &p[2]);
            if(isspace(buff[1])) {  //v x y z [w] but I do not support w yet
                if (components<3)
                    goto error;
                tVertices.push_back(p);
            }
            if (buff[1]=='n'){     //vn x y z
                if(components!=3)
                    goto error;
                tNormals.push_back(p);
            }
            else if (buff[1]=='t'){ //vt u [v] [w]
                if(components<1)
                    goto error;
                tTexCoords.push_back(p);
            }
        }else if (buff[0]=='f'){
            //end of vertex definition
            combineIndices.resize(tVertices.size());

            if (tNormals.size()>1)
                hasNormal=true;
            if (tTexCoords.size()>1)
                hasTexCoord=true;
            break;
        }
    }

    do{
        if (buff[0]!='f')
            continue;
        token = strtok_r(buff+2, " \t", &saved_ptr);
        for(int i=0;token;i++, token = strtok_r(NULL, " \n\t", &saved_ptr)){
            if (hasNormal && !hasTexCoord) //special model f v//vn
                sscanf(token,"%d//%d", &combineIndex.v, &combineIndex.vn);
            else
                sscanf(token,"%d/%d/%d", &combineIndex.v, &combineIndex.vt, &combineIndex.vn);

            index=findIndex(combineIndices, combineIndex, currIndex);

            if (i==0){
                firstIndex=index;
            }else if (i==1){
                prevIndex=index;
            }else if (i>=2){
                ptm->indices.push_back(firstIndex);
                ptm->indices.push_back(prevIndex);
                ptm->indices.push_back(index);
                prevIndex=index;
            }
        }
    }
    while((linelen=getline(&buff, &buffLen, f))!=-1);

    ptm->vertices.resize(currIndex);
    if (hasNormal){
        ptm->hasNormal=true;
        ptm->normals.resize(currIndex);
    }
    if (hasTexCoord){
        ptm->hasTexCoord=true;
        ptm->texCoords.resize(currIndex);
    }

    for(unsigned int i=0; i<tVertices.size(); ++i){
        CombinedIndex* cv = combineIndices[i];
        CombinedIndex* pv=cv;
        while (cv){
            int index = cv->index;
            if (cv->v <= 0 || cv->v >= tVertices.size())
                goto error;
            ptm->vertices[index]=tVertices[cv->v];
            if (hasNormal){
                if (cv->vn <= 0 || cv->vn >= tNormals.size())
                    goto error;
                ptm->normals[index]=tNormals[cv->vn];
            }
            if (hasTexCoord){
                if (cv->vt <= 0 || cv->vt >= tTexCoords.size())
                    goto error;
                ptm->texCoords[index]=tTexCoords[cv->vt];
            }
            pv=cv;
            cv=cv->next;
            free(pv);
        }
    }

    ptm->genGLbuffer();
    geometries.push_back(new Geometry(ptm, pm, indexStart, ptm->indices.size()-indexStart));
    free(buff);
    return true;

error:
    qDebug()<<"Error process file:"<<fileName.c_str()<<" position:" << ftell(f);
    free(buff);
    return false;

}
