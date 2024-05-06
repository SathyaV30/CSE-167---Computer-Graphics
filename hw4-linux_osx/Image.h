#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <glm/glm.hpp>
using glm::vec3;
class Image {
public:
    int width;
    int height;

    Image(int w, int h) : width(w), height(h), pixels(w * h) {}

    void setPixel(int x, int y, const vec3& color) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            pixels[y * width + x] = color;
        }
    }

    vec3 getPixel(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return pixels[y * width + x];
        }
        return vec3(0.0f);
    }

private:
    std::vector<vec3> pixels;
};

#endif