#ifndef RAY_H
#define RAY_H
#include <glm/glm.hpp>
#include "Camera.h"
using glm::vec3;


struct Ray {
    vec3 origin;   
    vec3 direction;

    Ray() : origin(0.0f), direction(0.0f, 0.0f, 1.0f) {}

    Ray(const vec3& o, const vec3& d) : origin(o), direction(d) {}

    vec3 pointAt(float t) const {
        return origin + t * direction;
    }

    
};
Ray generateRay(const Camera& camera, float x, float y);

#endif