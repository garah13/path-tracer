#include "pathtracer.h"
#include "iostream"
#include "scene/material/material.h"

using namespace Eigen;

PathTracer::PathTracer(int width, int height) : m_width(width), m_height(height)
{

}


void PathTracer::traceScene(const Scene &scene, QRgb *data) {
    Vector3f *rad = new Vector3f[m_width * m_height];
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            Vector3f color = tracePixel(scene, x, y);
            rad[y * m_width + x] = color;
        }
        std::cout << y << std::endl;

    }

    toneMap(data, rad);
    delete[] rad;
}

void PathTracer::toneMap(QRgb *data, Vector3f *radiance) {
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            int index = y * m_width + x;
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

    Eigen::Matrix4f invViewMatrix = (scene.getCamera()->getScaleMatrix() * scene.getCamera()->getViewMatrix()).inverse();
    Vector3f d((2.f * (x) / m_width) - 1.f, 1.f - (2.f * (y) / m_height), -1.f);
    Vector3f p(0, 0, 0);
    d.normalize();
    Ray r(p, d);
    Ray ray(r.transform(invViewMatrix));
    return traceRay(scene, ray);

}

Vector3f PathTracer::traceRay(const Scene &scene, const Ray &r) {
    IntersectionInfo info = IntersectionInfo();

    //if there is an intersection return red
    if (scene.getIntersection(r, info)) {
//        return Vector3f(1, 0, 0);
        Triangle *triangle = static_cast<Triangle *>(info.data);
        const MtlMaterial mtlMat = static_cast<const Mesh *>(info.obj)->getMaterial(triangle->getIndex());
        return mtlMat.diffuse;
//        if (mtlMat.emissiveness.norm() > 0.f) {
//            return mtlMat.emissiveness;
//        }
//        Vector3f normal = triangle->getNormal(info);
//        Material *mat = Material::material(mtlMat);
//        if (ProbUtil::random(0, 1) < .5f) {
//            SampleInfo sample = mat->sampleRay(r, normal, mtlMat);
//            Ray outgoing(r.origin, sample.direction);
//            Vector3f bsdf = mat->bsdf(r, outgoing, normal, mtlMat);
//            return bsdf * sample.direction.dot(normal) / (sample.prob * .5f);
//        }
    }

    return Vector3f(0, 0, 0);
}
