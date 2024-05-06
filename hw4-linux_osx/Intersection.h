#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "Ray.h"
#include <glm/glm.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::cross;
using glm::dot;
using glm::normalize;
using glm::inverse;
using glm::transpose;

#include <limits>

struct Intersection {
    vec3 point;
    vec3 normal;
    float distance;
    bool hit;
    vec3 barycentric;
    int objectIndex;

   Ray ray;

    Intersection() : point(0.0f), normal(0.0f), distance(std::numeric_limits<float>::max()), hit(false), barycentric(0.0f), objectIndex(-1) {}
};



Intersection intersectTriangle(const Ray& ray, const vec3& v0, const vec3& v1, const vec3& v2, const mat4& transform) {
    //This is an adaptation of the Möller–Trumbore intersection algorithm
    //Source: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    Intersection intersection;
    intersection.hit = false;
    intersection.ray = ray;

    mat4 invTransform = inverse(transform);
    vec3 transformedOrigin = vec3(invTransform * vec4(ray.origin, 1.0f));
    vec3 transformedDirection = vec3(invTransform * vec4(ray.direction, 0.0f));
    Ray transformedRay(transformedOrigin, transformedDirection);

    vec3 ab = v1 - v0;
    vec3 ac = v2 - v0;
    vec3 pvec = cross(transformedRay.direction, ac);
    float det = dot(ab, pvec);

    if (det >= -1e-7 && det <= 1e-7) {
        return intersection;
    }

    float invDet = 1.0f / det;
    vec3 tvec = transformedRay.origin - v0;
    float u = dot(tvec, pvec) * invDet;
    if (u < 0.0f || u > 1.0f) {
        return intersection;
    }

    vec3 qvec = cross(tvec, ab);
    float v = dot(transformedRay.direction, qvec) * invDet;
    if (v < 0.0f || u + v > 1.0f) {
        return intersection;
    }

    float t = dot(ac, qvec) * invDet;
    if (t < 0) {
        return intersection;
    }

    vec3 intersectionPoint = transformedRay.origin + t * transformedRay.direction;
    vec3 normal = normalize(cross(ab, ac));

    intersection.point = vec3(transform * vec4(intersectionPoint, 1.0f));
    intersection.normal = normalize(vec3(transpose(invTransform) * vec4(normal, 0.0f)));
    intersection.distance = length(intersection.point - ray.origin);
    intersection.hit = true;

    return intersection;
}

Intersection intersectSphere(const Ray& ray, const vec3& center, float radius, const mat4& transform) {
    
    Intersection intersection;
    intersection.hit = false;
    intersection.ray = ray;

    mat4 invTransform = inverse(transform);
    vec3 transformedOrigin = vec3(invTransform * vec4(ray.origin, 1.0f));
    vec3 transformedDirection = vec3(invTransform * vec4(ray.direction, 0.0f));
    Ray transformedRay(transformedOrigin, transformedDirection);

    vec3 l = transformedRay.origin - center;
    float a = dot(transformedRay.direction, transformedRay.direction);
    float b = 2.0f * dot(transformedRay.direction, l);
    float c = dot(l, l) - (radius * radius);
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return intersection;
    }

    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = (-b + sqrtDiscriminant) / (2 * a);
    float t0 = (-b - sqrtDiscriminant) / (2 * a);

    float t = (t0 < t1) ? t0 : t1;
    if (t < 0) {
        t = (t0 < t1) ? t1 : t0;
        if (t < 0) {
            return intersection;
        }
    }

    vec3 intersectionPoint = transformedRay.origin + t * transformedRay.direction;
    vec3 normal = normalize(intersectionPoint - center);

    intersection.point = vec3(transform * vec4(intersectionPoint, 1.0f));
    intersection.normal = normalize(vec3(transpose(mat3(invTransform)) * normal));
    intersection.distance = length(intersection.point - ray.origin);
    intersection.hit = true;

    return intersection;
}

#endif