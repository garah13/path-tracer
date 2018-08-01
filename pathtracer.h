#ifndef PATHTRACER_H
#define PATHTRACER_H


#include "QRgb"
#include "Eigen/Dense"
#include "scene/scene.h"
#include "AccelStructs/ray.h"
#include "AccelStructs/intersectioninfo.h"


class PathTracer
{
public:
    PathTracer(int width, int height);

    void traceScene(const Scene &scene, QRgb *data);


private:

    void toneMap(QRgb *data, Eigen::Vector3f *radiance);
    Eigen::Vector3f tracePixel(const Scene &scene, int x, int y);
    Eigen::Vector3f traceRay(const Scene &scene, const Ray &r);

    int m_width;
    int m_height;
};

#endif // PATHTRACER_H
