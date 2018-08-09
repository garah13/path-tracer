#include "probutil.h"

using namespace Eigen;




SampleInfo ProbUtil::uniformHemisphere() {
    float theta = 2 * M_PI * random(0.f, 1.f);
    float phi = std::acosf(1.f - random(0, 1.f));
    float x = std::sin(theta) * std::cos(phi);
    float y = std::sin(theta) * std::sin(phi);
    float z = std::cos(theta);
    return SampleInfo(1.f / (2 * M_PI), Vector3f(x, y, z));
}


//converts the sample in tangent space to world space with the
//normal as the up vector (assumed to be z in this case)
Vector3f ProbUtil::tangentToWorldSpace(const Vector3f &sample, const Vector3f &normal) {
    Vector3f vec = std::fabs(normal.x()) > .1f ? Vector3f(0, 1, 0) : Vector3f(1, 0, 0);
    Vector3f tangent = normal.cross(vec).normalized();
    Vector3f bitangent = normal.cross(tangent).normalized();
    return (sample(0) * tangent + sample(1) * bitangent + sample(2) * normal).normalized();
}




float ProbUtil::random(float low, float high) {
    return low + static_cast<float>(rand()) /static_cast<float>(RAND_MAX) * (high - low);
}
