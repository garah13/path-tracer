#ifndef OBJPARSER_H
#define OBJPARSER_H


#include <istream>
#include <vector>
#include "Eigen/Dense"
#include "scene/material.h"
#include "map"
#include <regex>


class ObjParser
{

public:
    ObjParser();

    static void LoadObj(const char* filename, std::vector<Eigen::Vector3f> &vertices, std::vector<Eigen::Vector3f> &normals,
                        std::vector<Eigen::Vector3i> &faces, std::vector<Eigen::Vector3i> &faceNormals,
                        std::map<std::string, int> &materialIds, std::vector<Material> &materials, std::vector<int> &faceMaterialId);


    //need to have a map of material to file name
    static void loadMtl(const char* filename, std::map<std::string, int> &materialIds, std::vector<Material> &materials);

    static void getLine(std::istream& is, std::string& res);

    //functions to parse vertices, normals and faces
    static void parseVertex(const char *token, float &x, float &y, float &z);
    static void parseNormal(const char *token, float &x, float &y, float &z);
    static void parseFace(const char *token, int &v1, int &v2, int &v3, int &vn1, int &vn2, int &vn3);


    //functions to parse strings, ints, and floats



    static bool parseInt(const char **token, int &value);
    static bool parseFloat(const char **token, float &value);
    static bool parseDouble(const char **token, double &value);

    static std::string parseString(const char **token);


private:

    static const std::regex intRegex;
    static const std::regex floatRegex;
    static const std::regex verts;
    static const std::regex faceVIndices;
    static const std::regex faceVNIndices;

};


#endif // OBJPARSER_H
