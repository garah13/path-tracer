#ifndef OBJPARSER_H
#define OBJPARSER_H


#include <istream>

class ObjParser
{

public:
    ObjParser();

    static void LoadObj(const char* filename);

    static void getLine(std::istream& is, std::string& res);

    //functions to parse vertices, normals and faces
    static void parseVertex(const char *token, float &x, float &y, float &z);
    static void parseNormal(const char *token, float &x, float &y, float &z);
    static void parseFace(const char *token, int &v1, int &v2, int &v3);


    //functions to parse strings, ints, and floats
    static int parseInt(const char **token);
    static float parseFloat(const char **token);
    static double parseDouble(const char **token);
    static std::string parseString(const char **token);

    //checks if the the characters up to index = length - 1
    //are digits
    static bool isDigit(const char *token, int length);

};


#endif // OBJPARSER_H
