#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "QRunnable"
#include "pathtracer.h"

class RenderThread : public QRunnable {
public:
    RenderThread();

protected:

    PathTracer *m_pathTracer;
    const Scene *m_scene;
    QRgb *m_imageData;

};

#endif // RENDERTHREAD_H
