#include "objparser.h"

#include <fstream>
#include <iostream>


using namespace Eigen;

//const static member variables
const std::regex ObjParser::intRegex("[0-9]+");

const std::regex ObjParser::floatRegex("(?:-)?[0-9]+(?:\\.[0-9]+)?");
const std::regex ObjParser::verts("[[:space:]]*(?:(?:-)?[0-9]+(?:\\.[0-9]+)?[[:space:]]+){2}(?:-)?[0-9]+(?:\\.[0-9]+)?[[:space:]]*");
const std::regex ObjParser::faceVIndices("[[:space:]]*(?:[0-9]+[[:space:]]+){2}[0-9]+[[:space:]]*");
const std::regex ObjParser::faceVNIndices("[[:space:]]*(?:[0-9]+\/\/[0-9]+[[:space:]]+){2}[0-9]+\/\/[0-9]+[[:space:]]*");


ObjParser::ObjParser() {
}


//first thing is to only parse out vertex coordinates
//then normals
//etc.
void ObjParser::LoadObj(const char *filename, std::vector<Vector3f> &vertices, std::vector<Vector3f> &normals, std::vector<Vector3i> &faces,
                        std::vector<Vector3i> &faceNormals, std::map<std::string, int> &materialIds, std::vector<Material> &materials, std::vector<int> &faceMaterialId) {

    std::ifstream ifs(filename, std::ifstream::in);
    if (!ifs) {
        std::cout << "file could not be opened: " << filename << std::endl;
        std::exit(1);
    }

    int currMaterialId = -1;

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
        if (token[0] == 'v' && token[1] == ' ') {
            token += 2; //pointer arithmetic move 2 forward
            Vector3f v;
            parseVertex(token, v(0), v(1), v(2));
            vertices.push_back(v);
            faceMaterialId.push_back(currMaterialId);
            continue;
        }

        //case for NORMAL
        if (token[0] == 'v' && token[1] == 'n') {
            token += 2;
            Vector3f n;
            parseNormal(token, n(0), n(1), n(2));
            normals.push_back(n );
            continue;
        }

        //cases for faces
        if (token[0] == 'f' && token[1] == ' ') {
            token += 2;
            Vector3i f;
            Vector3i fn;

            parseFace(token, f(0), f(1), f(2), fn(0), fn(1), fn(2));
            faces.push_back(f);
            faceNormals.push_back(fn);
            continue;
        }

        //loads the mtl file
        if (strncmp(token, "mtllib", 6) == 0 && token[6] == ' ') {
            token += 7;
            std::stringstream matFileName;
            matFileName << token;
            const char *fn = matFileName.str().c_str();
            loadMtl(fn, materialIds, materials);
            continue;
        }

        //sets current material for the faces read in
        if (strncmp(token, "usemtl", 6) == 0 && token[6] == ' ') {
            token += 7;
            std::stringstream ss;
            ss << token;
            std::string name = ss.str();
            if (materialIds.find(name) != materialIds.end()) {
                currMaterialId = materialIds[name];
            } else {
                currMaterialId = -1;
            }
            continue;
        }
    }

    //for every line

    //get first character after trimming whitespace from both endputs
    //look for key characters
    //write up parsing int, parsing real/float, etc.
    //only support decimals now
}

void ObjParser::loadMtl(const char* filename, std::map<std::string, int> &materialIds, std::vector<Material> &materials) {
    std::ifstream ifs(filename, std::ifstream::in);
    if (!ifs) {
        std::cout << "file could not be opened: " << filename << std::endl;
        std::exit(1);
    }

    Material mat;
    mat.flush();
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

        if (strncmp(token, "newmtl", 6) == 0 && token[6] == ' ') {
            if (!mat.name.empty()) {
                materialIds.insert(std::pair<std::string, int>(mat.name, static_cast<int>(materials.size())));
                materials.push_back(mat);
            }
            mat.flush();

            //TODO:: get new name and set as material name
            token += 7;
            std::stringstream sstr;
            sstr << token;
            mat.name = sstr.str();
            continue;
        }

        //TODO:: fill in material stuff
        if (strncmp(token, "Ka", 2) == 0 && token[2] == ' ') {
            token += 2;
            parseVertex(token, mat.ambient(0), mat.ambient(1), mat.ambient(2));
            continue;
        }

        if (strncmp(token, "Kd", 2) == 0 && token[2] == ' ') {
            token += 2;
            parseVertex(token, mat.diffuse(0), mat.diffuse(1), mat.diffuse(2));
            continue;
        }

        if (strncmp(token, "Ks", 2) == 0 && token[2] == ' ') {
            token += 2;
            parseVertex(token, mat.specular(0), mat.specular(1), mat.specular(2));
            continue;
        }

        if (strncmp(token, "Tf", 2) == 0 && token[2] == ' ') {
            token += 2;
            parseVertex(token, mat.transmittance(0), mat.transmittance(1), mat.transmittance(2));
            continue;
        }

        if (strncmp(token, "Ke", 2) == 0 && token[2] == ' ') {
            token += 2;
            parseVertex(token, mat.emissiveness(0), mat.emissiveness(1), mat.emissiveness(2));
            continue;
        }

        if (strncmp(token, "Ns", 2) == 0 && token[2] == ' ') {
            token += 2;
            parseFloat(&token, mat.shininess);
            continue;
        }

        if (strncmp(token, "Ni", 2) == 0 && token[2] == ' ') {
            token += 2;
            parseFloat(&token, mat.ior);
            continue;
        }
    }
    if (!mat.name.empty()) {
        materialIds[mat.name] = materials.size();
        materials.push_back(mat);
    }
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

//parse a string
std::string ObjParser::parseString(const char **token) {
    token += strspn(*token, " \t");
    int size = strcspn(*token, " \t\r");
    std::string value(*token, size);
    token += size;
    return value;
}

//parse int
bool ObjParser::parseInt(const char **token, int &value) {
    *token += strspn(*token, " \t");
    std::cmatch cm;
    if (std::regex_search(*token, cm, intRegex)) {
        value = atoi(*token);
        *token += cm.length(0);
        return true;
    }
    return false;
}

//parse float
bool ObjParser::parseFloat(const char **token, float &value) {
    *token += strspn(*token, " \t");
    std::cmatch cm;
    if (std::regex_search(*token, cm, floatRegex)) {
        value = strtof(*token, NULL);
        *token += cm.length(0);
        return true;
    }
    return false;
}

//parse Vertex information
void ObjParser::parseVertex(const char *token, float &x, float &y, float &z) {
    std::cmatch cm;
    if (std::regex_match(token, cm, verts)) {
        parseFloat(&token, x);
        parseFloat(&token, y);
        parseFloat(&token, z);
    }
}


//parse vertex normal information
void ObjParser::parseNormal(const char *token, float &x, float &y, float &z) {
    std::cmatch cm;
    if (std::regex_match(token, cm, verts)) {
        parseFloat(&token, x);
        parseFloat(&token, y);
        parseFloat(&token, z);
    }
}


//parse face information
//i understand the index offset, tells you what to add
//like the buffers
void ObjParser::parseFace(const char *token, int &v1, int &v2, int &v3, int &vn1, int &vn2, int &vn3) {
    std::cmatch cm;
    if (std::regex_match(token, cm, faceVIndices)) {
        parseInt(&token, v1);
        parseInt(&token, v2);
        parseInt(&token, v3);
        vn1 = vn2 = vn3 = -1;
        v1 -= 1;
        v2 -= 1;
        v3 -= 1;
    }

    if (std::regex_match(token, cm, faceVNIndices)) {
        parseInt(&token, v1);
        token += 2;
        parseInt(&token, vn1);
        parseInt(&token, v2);
        token += 2;
        parseInt(&token, vn2);
        parseInt(&token, v3);
        token += 2;
        parseInt(&token, vn3);
        v1 -= 1;
        v2 -= 1;
        v3 -= 1;
        vn1 -= 1;
        vn2 -= 1;
        vn3 -= 1;

    }
}


