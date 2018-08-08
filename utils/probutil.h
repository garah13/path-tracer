#ifndef PROBUTIL_H
#define PROBUTIL_H

#include "Eigen/Dense"

struct SampleInfo {
    SampleInfo() : prob(0), direction(Eigen::Vector3f(0, 0, 0)) {}
    SampleInfo(float prob, Eigen::Vector3f direction) : prob(prob), direction(direction) {}

    float prob;
    Eigen::Vector3f direction;

};


class ProbUtil
{
public:

    static SampleInfo uniformHemisphere();

    static float random(float low, float high);
    static Eigen::Vector3f tangentToWorldSpace(const Eigen::Vector3f &sample, const Eigen::Vector3f &normal);

};

#endif // PROBUTIL_H
