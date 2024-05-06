#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"

struct Camera {
    vec3 eye;        
    vec3 center;      
    vec3 up;        
    float fovy;      
    int width;       
    int height;       

    Camera() {
        eye = vec3(0.0f, 0.0f, 0.0f);
        center = vec3(0.0f, 0.0f, 0.0f);
        up = vec3(0.0f, 1.0f, 0.0f);
        fovy = 90.0f;
        width = 500;
        height = 500;
    }

    Camera(const vec3& e, const vec3& c, const vec3& u, float f, int w, int h)
        : eye(e), center(c), up(u), fovy(f), width(w), height(h) {}
};

#endif