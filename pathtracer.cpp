#include "pathtracer.h"

using namespace Eigen;

PathTracer::PathTracer(int width, int height) : m_width(width), m_height(height)
{

}


void PathTracer::traceScene(const Scene &scene, QRgb *data) {
    Vector3f *rad = new Vector3f[m_width * m_height];
    for (int i = 0; i < m_width; i++) {
        for (int j = 0; j < m_height; j++) {
            Vector3f color = tracePixel(scene, i, j);
            rad[i * m_width + j] = color;
        }
    }

    toneMap(data, rad);
    delete[] rad;
}

void PathTracer::toneMap(QRgb *data, Vector3f *radiance) {
    for (int i = 0; i < m_width; i++) {
        for (int j = 0; j < m_height; j++) {
            int index = i * m_width + j;
            Vector3f intensity = radiance[index];
            Vector3f intensityVal = Vector3f(std::fmax(0, intensity[0]), std::fmax(0, intensity[1]), std::fmax(0, intensity[2]));
            Vector3f add = Vector3f(1, 1, 1) + intensityVal;
            intensityVal = intensityVal.cwiseQuotient(add);
            intensityVal *= 255;
            data[index] = qRgb(intensityVal(0), intensityVal(1), intensityVal(2));
        }
    }
}

Vector3f PathTracer::tracePixel(const Scene &scene, int x, int y) {

    Eigen::Matrix4Xf invViewMatrix = (scene.getCamera()->getScaleMatrix() * scene.getCamera()->getViewMatrix()).inverse();
    Vector3f d = Vector3f(2.f * (x / m_width) - 1.f, 1.f - 2.f * (y / m_height), -1);
    d.normalize();
    Ray r(scene.getCamera()->getEye(), d);
    Ray ray = r.transform(invViewMatrix);
    return traceRay(scene, ray);

}

Vector3f PathTracer::traceRay(const Scene &scene, const Ray &r) {
    IntersectionInfo info = IntersectionInfo();

    //if there is an intersection return red
    if (scene.getIntersection(r, info)) {
        return Vector3f(1, 0, 0);
    }

    return Vector3f(0, 0, 0);
}
