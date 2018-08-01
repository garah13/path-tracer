#include <QCoreApplication>
#include "QImage"
#include "shapes/objparser.h"
#include "scene/xmlsceneparser.h"
#include "scene/scene.h"
#include <iostream>

#include "regex"
#include "Eigen/Dense"
using namespace Eigen;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::string filename("/Users/sarahgilmore/Documents/code/pathtracer/scenefiles/untitled.obj");
    std::vector<Vector3f> verts;
    std::vector<Vector3f> normals;
    std::vector<Vector3i> faces;
    std::vector<Vector3i> faceNormals;

    ObjParser::LoadObj(filename.c_str(), verts, normals, faces, faceNormals);

    for (int i = 0; i < static_cast<int>(verts.size()); i++) {
        std::cout << "v " << verts[i][0] << " " << verts[i][1] << " " << verts[i][2] << std::endl;
    }
    std::cout << std::endl;
    for (int i = 0; i < static_cast<int>(normals.size()); i++) {
        std::cout << "vn " << normals[i][0] << " " << normals[i][1] << " " << normals[i][2] << std::endl;
    }
    std::cout << std::endl;

    for (int i = 0; i < static_cast<int>(faces.size()); i++) {
        std::cout << "f " << faces[i][0] << " " << faces[i][1] << " " << faces[i][2] << std::endl;
    }
    std::cout << std::endl;

    for (int i = 0; i < static_cast<int>(faceNormals.size()); i++) {
        std::cout << "fn " << faceNormals[i][0] << " " << faceNormals[i][1] << " " << faceNormals[i][2] << std::endl;
    }

    a.exit(0);
}
