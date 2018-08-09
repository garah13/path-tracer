#include <QCoreApplication>
#include "QImage"
#include "shapes/objparser.h"
#include "scene/xmlsceneparser.h"
#include "scene/scene.h"
#include <iostream>
#include "pathtracer.h"

#include "regex"
#include "Eigen/Dense"
using namespace Eigen;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    Scene *scene = new Scene();
    scene->load("/Users/sarahgilmore/Documents/code/pathtracer/scenefiles/test.xml");
    QImage image(400, 400, QImage::Format_RGB32);
    PathTracer tracer(400, 400);

    QRgb *data = reinterpret_cast<QRgb *>(image.bits());
    tracer.traceScene(*scene, data);
    bool success = image.save("/Users/sarahgilmore/Documents/code/pathtracer/scenefiles/TEST2.png");
    if(success) {
        success = image.save("/Users/sarahgilmore/Documents/code/pathtracer/scenefiles/TEST2.png", "PNG");
    }

    a.exit(0);
}
