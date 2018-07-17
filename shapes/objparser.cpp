#include "objparser.h"

#include <fstream>
#include <iostream>

ObjParser::ObjParser()
{

}


//first thing is to only parse out vertex coordinates
//then normals
//etc.
void ObjParser::LoadObj(const char *filename) {

    std::ifstream ifs(filename, std::ifstream::in);
    if (!ifs) {
        std::cout << "file could not be opened: " << filename << std::endl;
        std::exit(1);
    }

    while (ifs.peek() != EOF) {
        std::string linebuf;
        getLine(ifs, linebuf);

        //trim line
        if (linebuf.size() > 0) {
            if (linebuf[linebuf.size() - 1] == '\n') {
                linebuf.erase(linebuf.size() - 1);
            }
        }
        if (linebuf.size() > 0) {
            if (linebuf[linebuf.size() - 1] == '\r') {
                linebuf.erase(linebuf.size() - 1);
            }
        }

        //empty line
        if (linebuf.empty()) {
            continue;
        }

        //skip any leading whitespace
        const char *token = linebuf.c_str();
        token += strspn(token, " \t");

        //case for COMMENT
        if (token[0] == '#') {
            continue;
        }

        //case for VERTEX
        if (token[0] == 'v' && token[0] == ' ') {
            token += 2; //pointer arithmetic move 2 forward
            float x, y, z;
            parseVertex(token, x, y, z);
            continue;
        }

        //case for NORMAL
        if (token[0] == 'v' && token[1] == 'n') {
            token += 2;
            float x, y, z;
            parseNormal(token, x, y, z);
            continue;
        }

        if (token[0] == 'f' && token[1] == ' ') {
            token += 2;
            int v1, v2, v3;
            parseFace(token, v1, v2, v3);
            continue;
        }
    }

    //for every line

    //get first character after trimming whitespace from both endputs
    //look for key characters
    //write up parsing int, parsing real/float, etc.
    //only support decimals now
}

//do I need to return this?
void ObjParser::getLine(std::istream &is, std::string &res) {

    std::istream::sentry se(is, true);
    std::streambuf* buf = is.rdbuf();

    //runs until the line is read
    //potential forever loop
    while (true) {
        int c = buf->sbumpc();
        switch(c) {
        case '\n':
            return;
        case '\r':
            if (buf->sgetc() == '\n') {
                buf->sbumpc();
            }
            return;
        case std::streambuf::traits_type::eof():
            if (res.empty()) {
                is.setstate(std::ios::eofbit);
            }
            return;
        default:
            res += static_cast<char>(c);
            break;
        }
    }
}

//parse integer
int ObjParser::parseInt(const char **token) {
    *token += strspn(*token, " \t");
    int value = atoi(*token);
    *token += strcspn(*token, " \t\r");
    return value;
}

//parse float
float ObjParser::parseFloat(const char **token) {
    *token += strspn(*token, " \t");
    float value = strtof(*token, NULL);
    *token += strcspn(*token, " \t\r");
    return value;
}

//parse a double
double ObjParser::parseDouble(const char **token) {
    *token += strspn(*token, " \t");
    double value = strtod(*token, NULL);
    *token += strcspn(*token, " \t\r");
    return value;
}

//parse a string
std::string ObjParser::parseString(const char **token) {
    token += strspn(*token, " \t");
    int size = strcspn(*token, " \t\r");
    std::string value(*token, size);
    token += size;
    return value;
}

//checks if every character up to length minus 1 is
//a digit. If so, returns true. Otherwise returns false.
//if length is 0, returns true
bool ObjParser::isDigit(const char *token, int length) {
    for (int i = 0; i < length; i++) {
        if (!isdigit(token[i])) {
            return false;
        }
    }
    return true;
}

//parse Vertex information
void ObjParser::parseVertex(const char *token, float &x, float &y, float &z) {
    x = parseFloat(&token);
    if (token[0] == '\0') {
        std::cout << "requires three numbers per vertex" << std::endl;
        exit(1);
    }
    y = parseFloat(&token);
    if (token[0] == '\0') {
        std::cout << "requires three numbers per vertex" << std::endl;
        exit(1);
    }
    z = parseFloat(&token);
}

//parse vertex normal information
void ObjParser::parseNormal(const char *token, float &x, float &y, float &z) {
    x = parseFloat(&token);
    if (token[0] == '\0') {
        std::cout << "requires three numbers per normal" << std::endl;
        exit(1);
    }
    y = parseFloat(&token);
    if (token[0] == '\0') {
        std::cout << "requires three numbers per normal" << std::endl;
        exit(1);
    }
    z = parseFloat(&token);
}

//parse face information
void ObjParser::parseFace(const char *token, int &v1, int &v2, int &v3) {
    v1 = parseInt(&token) - 1;
    if (token[0] == '\0') {
        std::cout << "requires three vertices per face" << std::endl;
        exit(1);
    }
    v2 = parseInt(&token) - 1;
    if (token[0] == '\0') {
        std::cout << "requires three vertices per face" << std::endl;
        exit(1);
    }
    v3 = parseInt(&token) - 1;
}


