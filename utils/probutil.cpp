#include "probutil.h"
#include <cmath>

using namespace Eigen;



//check this
SampleInfo ProbUtil::uniformHemisphere() {
    float phi = 2 * M_PI * random(0.0f, 1.0f);
    float theta = std::acos(1.f - random(0.0f, 1.0f));
    float x = std::sin(theta)*std::cos(phi);
    float y = std::sin(theta)*std::sin(phi);
    float z = std::cos(theta);
    return SampleInfo(1.f / (2 * M_PI), Vector3f(x, y, z));
}


//converts the sample in tangent space to world space with the
//normal as the up vector (assumed to be z in this case)
Vector3f ProbUtil::tangentToWorldSpace(const Vector3f &sample, const Vector3f &normal) {
    Vector3f vec = normal(0) > .1f || normal(0) < -.1f ? Vector3f(0, 1, 0) : Vector3f(1, 0, 0);
    Vector3f tangent = normal.cross(vec).normalized();
    Vector3f bitangent = normal.cross(tangent).normalized();
    return (sample(0) * tangent + sample(1) * bitangent + sample(2) * normal).normalized();
}


float ProbUtil::random(float low, float high) {
    return low + static_cast<float>(rand()) /static_cast<float>(RAND_MAX) * (high - low);
}

