#ifndef SCENE_H
#define SCENE_H

#include "Camera.h"
#include <vector>

struct Scene {
    Camera camera;
    vec3 ambient;
    vec3 attenuation;
    int maxDepth;

    Scene() : ambient(0.2f, 0.2f, 0.2f), attenuation(1.0f, 0.0f, 0.0f), maxDepth(5) {}
};

#endif