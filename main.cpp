#include <QCoreApplication>
#include "shapes/objparser.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    std::string value(" 123 a");
//    const char *token = value.c_str();
//    float val = ObjParser::parseFloat(&token);
//    std::cout << "parsed val : " << val << std::endl;
//    std::cout << token << std::endl;

    std::string value("1.5 4.5 5");
    const char *token = value.c_str();
    float x, y, z;
    ObjParser::parseVertex(token, x, y, z);
    a.exit(0);
}
