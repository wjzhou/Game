#include "objparser.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <tr1/array>
#include "common.hpp"
#include <QtDebug>

ObjParser::ObjParser(TriangleMesh& atm)
    :tm(atm)
{
}

static int findIndex(bool &needInit, int fixedArea,
                     std::vector<std::tr1::array<int,3> >& combineIndices,
                     std::tr1::array<int,3>& combineIndex)
{
    int i;
    needInit=false;
    int v=combineIndex[0];
    if (combineIndices[v][0]==0){
        combineIndices[v]=combineIndex;
        needInit=true;
        return v;
    }

    if(combineIndices[v] == combineIndex)
        return v;
    for (i=fixedArea;i<combineIndices.size();i++){
        if (combineIndices[i] == combineIndex)
            return i;
    }
    combineIndices.push_back(combineIndex);
    needInit=true;
    return i;
}

bool ObjParser::parse(std::string fileName){
    FILE* f;
    char* buff=NULL;
    size_t buffLen=0;
    size_t linelen;

    std::vector<glm::vec3> tVertices;
    std::vector<glm::vec3> tNormals;
    std::vector<glm::vec3> tTexCoords;
    std::vector<glm::uint> tIndices;
    //std::vector<attribute> attributes;
    std::vector<std::tr1::array<int,3> > combineIndices;
    std::tr1::array<int,3> combineIndex;

    bool needInit;
    int fixedArea;
    int components;
    int v,vn,vt;
    int firstIndex, prevIndex;
    int index;

    char* saved_ptr; //used by strtok_r
    char* token;

    glm::vec3 zero(0.0f, 0.0f, 0.0f);
    glm::vec3 p=zero;
    tVertices.push_back(zero); //obj file index start from 1
    tNormals.push_back(zero);
    tTexCoords.push_back(zero);

    tm.hasTexCoord=false;
    tm.hasNormal=false;

    if ((f=fopen(fileName.c_str(),"r"))==NULL){
        qDebug() << "can not open file " << fileName.c_str()
                 << ": no such file or directory";
      qDebug() << "Testing";
      return false;
    }

    while((linelen=getline(&buff, &buffLen, f))!=-1){ //use c language for
                                                      //better performance

        //char[50] fpart0,fpart1,fpart2,fpart3;
        if (buff[0]=='v'){
            p=glm::vec3(0.0f, 0.0f, 0.0f);
            components=sscanf(buff+2, "%f %f %f", &p[0], &p[1], &p[2]);
            if(isspace(buff[1])) {  //v x y z [w] but I do not support w yet
                if (components<3)
                    goto error;
                /*if (components==3)
                    p[3]=1.0f;*/
                tVertices.push_back(p);
            }
            if (buff[1]=='n'){     //vn x y z
                if(components!=3)
                    goto error;
                    //return false;
                tNormals.push_back(p);
            }
            else if (buff[1]=='t'){ //vt u [v] [w]
                if(components<1)
                    goto error;
                tTexCoords.push_back(p);
            }
        }else if (buff[0]=='f'){
            //end of vertex definition
            fixedArea=tVertices.size();
            combineIndices.resize(fixedArea);
            tm.vertices.resize(fixedArea);

            combineIndex[0]=combineIndex[1]=combineIndex[2]=0;
            for (int i=0;i<tVertices.size();i++){
                combineIndices[i]=combineIndex;
            }

            if (tTexCoords.size()>1){
                tm.hasTexCoord=true;
                tm.texCoords.resize(fixedArea);
            }
            if (tNormals.size()>1){
                tm.hasNormal=true;
                tm.normals.resize(fixedArea);
            }
            break;
        }
    }

    do{
        if (buff[0]=='#' or buff[0]=='\n')
            continue;
        if (buff[0]!='f')
            goto error;
        token = strtok_r(buff+2, " \t", &saved_ptr);
        for(int i=0;token;i++, token = strtok_r(NULL, " \n\t", &saved_ptr)){
           /* if (token == NULL)
                break;    */
            v=vt=vn=0;
            if (tm.hasNormal && !tm.hasTexCoord) //special model f v//vn
                sscanf(token,"%d//%d", &v, &vn);
            else
                sscanf(token,"%d/%d/%d", &v, &vt, &vn);
            combineIndex[0]=v;
            combineIndex[1]=vt;
            combineIndex[2]=vn;
            //boost use this strange double brace syntax..
            //although it says standard compiler can deal with one brace
            //std::tr1::array<int,3> combineIndex={{v,vt,vn}};
            index=findIndex(needInit, fixedArea,
                                combineIndices, combineIndex);
            //int* combineIndex=&combineIndices[v];
            if (needInit){ //first time use the combine
                if (v>=tVertices.size()){
                    qDebug()<<"vertex index:"<<v<<" out of range";
                    goto error;
                }
                if(index<fixedArea)
                    tm.vertices[index]=tVertices[v];
                else
                    tm.vertices.push_back(tVertices[v]);
                if (tm.hasTexCoord){
                    if (vt>=tTexCoords.size()){
                        qDebug()<<"textual index:"<<vt<<" out of range";
                        goto error;
                    }
                    if(index<fixedArea)
                        tm.texCoords[index]=tTexCoords[vt];
                    else
                        tm.texCoords.push_back(tTexCoords[vt]);
                }
                if (tm.hasNormal){
                    if (vn>=tNormals.size()){
                        qDebug()<<"normal index:"<<vn<<" out of range";
                        goto error;
                    }
                    if(index<fixedArea)
                        tm.normals[index]=tNormals[vn];
                    else
                        tm.normals.push_back(tNormals[vn]);
                }
            }
            if (i==0){
                firstIndex=index;
            }else if (i==1){
                prevIndex=index;
            }else if (i>=2){
                tm.indices.push_back(firstIndex);
                tm.indices.push_back(prevIndex);
                tm.indices.push_back(index);
                prevIndex=index;
            }
        }

    }
    while((linelen=getline(&buff, &buffLen, f))!=-1);
    free(buff);
    return true;

error:
    qDebug()<<"Error process file:"<<fileName.c_str()<<" position:" << ftell(f);
    free(buff);
    return false;

}
