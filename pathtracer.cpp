#include "pathtracer.h"
#include "iostream"
#include "scene/material/material.h"

using namespace Eigen;

PathTracer::PathTracer(int width, int height) : m_width(width), m_height(height) {
    srand(time(NULL));
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
    Vector3f color(0, 0, 0);
    Vector3f p(0, 0, 0);
    int num = 2;
    for (int i = 0; i < num; i++) {
        Vector3f d((2.f * (x + ProbUtil::random(-.5f, .5f)) / m_width) - 1.f, 1.f - (2.f * (y + ProbUtil::random(-.5f, .5f)) / m_height), -1.f);
        d.normalize();
        Ray r(p, d);
        Ray ray(r.transform(invViewMatrix));
        color += traceRay(scene, ray);
    }
    color /= num;
    return color;
}

Vector3f PathTracer::traceRay(const Scene &scene, const Ray &r) {
    IntersectionInfo info = IntersectionInfo();

    //if there is an intersection return red
    if (scene.getIntersection(r, info)) {
        Triangle *triangle = static_cast<Triangle *>(info.data);
        const MtlMaterial mtlMat = static_cast<const Mesh *>(info.obj)->getMaterial(triangle->getIndex());

        if (mtlMat.emissiveness.norm() > 0.f) {
            return mtlMat.emissiveness;
        }

        return mtlMat.diffuse;
        Vector3f normal = static_cast<const Mesh *>(info.obj)->getNormal(info);
        Material *mat = Material::material(mtlMat);
        if (ProbUtil::random(0, 1) < .3f) {
            SampleInfo sample = mat->sampleRay(r, normal, mtlMat);

            //move off the surface
            Ray outgoing(info.hit + normal * .0001f, sample.direction);
            Vector3f bsdf = mat->bsdf(r, outgoing, normal, mtlMat);
            float dot = sample.direction.dot(normal);
            Vector3f radiance = traceRay(scene, outgoing);
            delete mat;
            return radiance.cwiseProduct(bsdf) * std::fmax(0, dot) / (sample.prob * .3f);
        }
    }
    return Vector3f(0, 0, 0);
}
