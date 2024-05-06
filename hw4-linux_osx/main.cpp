/******************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi  */
/* Extends HW 1 to deal with shading, more transforms and multiple objects    */
/******************************************************************************/

// You shouldn't have to edit this file at all!

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "Transform.h"
#include <FreeImage.h>
#include <random>


using namespace std; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  
#include "Ray.h"
#include "Camera.h"
#include "Intersection.h"
#include "Image.h"
#include "Scene.h"



vec3 calculateShading(const Intersection& intersection, const object& obj, const vec3& cameraEye, const Scene& scene, int depth);



void rayTrace(const Camera& camera, Image& image, Scene& scene) {
    int completedPixels = 0;
    int totalPixels = image.height * image.width;

    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < image.width; x++) {
            float s = static_cast<float>(x) / (image.width - 1);
            float t = 1.0f - static_cast<float>(y) / (image.height - 1);
            Ray ray = generateRay(camera, s, t);

            Intersection nearestHit;
            for (int i = 0; i < numobjects; i++) {
                const object& obj = objects[i];
                Intersection hit;
                vec3 sphereCenter;
                switch (obj.type) {
                    case triangle:
                        hit = intersectTriangle(ray, vertices[obj.coords[0]], vertices[obj.coords[1]], vertices[obj.coords[2]], obj.transform);
                        break;
                    case sphere:
                        sphereCenter = vec3(obj.coords[0], obj.coords[1], obj.coords[2]);
                        hit = intersectSphere(ray, sphereCenter, obj.size, obj.transform);
                        break;
                    default:
                        break;
                }
                if (hit.hit && hit.distance < nearestHit.distance) {
                    nearestHit = hit;
                    nearestHit.objectIndex = i;
                }
            }

            if (nearestHit.hit) {
                const object& obj = objects[nearestHit.objectIndex];
                vec3 color = calculateShading(nearestHit, obj, camera.eye, scene, 0);
                image.setPixel(x, y, color);
            } else {
                
                image.setPixel(x, y, vec3(0.0f));
            }

            completedPixels++;

            float progress = static_cast<float>(completedPixels) / totalPixels * 100.0f;
            std::cout << "\rProgress: [";
            int barWidth = 50;
            int filledWidth = static_cast<int>(progress / 100.0f * barWidth);
            for (int i = 0; i < barWidth; i++) {
                if (i < filledWidth)
                    std::cout << "X";
                else
                    std::cout << " ";
            }
            std::cout << "] " << std::fixed << std::setprecision(2) << progress << "%";
            std::cout.flush();
        }
    }

    std::cout << "\nray tracing complete" << std::endl;
}

Ray generateRay(const Camera& camera, float s, float t) {
    float aspectRatio = static_cast<float>(camera.width) / static_cast<float>(camera.height);
    float fovyRadians = camera.fovy * pi / 180.0f;

    vec3 w = camera.eye - camera.center; 
    vec3 u = cross(camera.up, w);        
    vec3 v = cross(w, u);                

    float ndcX = (2.0f * s) - 1.0f;                  
    float ndcY = 1.0f - (2.0f * t);                 

    float alpha = tan(fovyRadians / 2.0f) * aspectRatio * ndcX;
    float beta = tan(fovyRadians / 2.0f) * ndcY;

    vec3 rayDirection = normalize(alpha * normalize(u) + beta * normalize(v) - normalize(w));

    return Ray(camera.eye, rayDirection);
}

bool traceShadowRay(const vec3& intersectionPoint, const vec3& lightPos, const Scene& scene) {
    vec3 shadowRayDir = normalize(lightPos - intersectionPoint);
    float epsilon = 0.001f; 

    float lightDistance = length(lightPos - intersectionPoint);

    Ray shadowRay(intersectionPoint + epsilon * shadowRayDir, shadowRayDir);

    for (int i = 0; i < numobjects; i++) {
        const object& obj = objects[i];
        Intersection hit;
        vec3 sphereCenter;

        switch (obj.type) {
            case triangle:
                hit = intersectTriangle(shadowRay, vertices[obj.coords[0]], vertices[obj.coords[1]], vertices[obj.coords[2]], obj.transform);
                break;
            case sphere:
                sphereCenter = vec3(obj.coords[0], obj.coords[1], obj.coords[2]);
                hit = intersectSphere(shadowRay, sphereCenter, obj.size, obj.transform);
                break;
            default:
                continue;
        }

        if (hit.hit) {
            if (hit.distance > epsilon && hit.distance < lightDistance) {
                return true; 
            }
        }
    }
    return false; 
}

vec3 calculateShading(const Intersection& intersection, const object& obj, const vec3& cameraEye, const Scene& scene, int depth) {
    vec3 ambient = vec3(obj.ambient[0], obj.ambient[1], obj.ambient[2]);
    vec3 emission = vec3(obj.emission[0], obj.emission[1], obj.emission[2]);
    vec3 diffuse(0.0f);
    vec3 specular(0.0f);
    for (int i = 0; i < numused; i++) {
        vec3 lightPos(lightposn[i * 4], lightposn[i * 4 + 1], lightposn[i * 4 + 2]);
        vec3 lightColor(lightcolor[i * 4], lightcolor[i * 4 + 1], lightcolor[i * 4 + 2]);
        float lightType = lightposn[i * 4 + 3]; 


        vec3 lightDir;
        float attenuation = 1.0f;

        if (lightType == 1.0f) {
            // Point light
            lightDir = normalize(lightPos - intersection.point);
            float distance = length(lightPos - intersection.point);
            attenuation = 1.0f / (scene.attenuation[0] + scene.attenuation[1] * distance + scene.attenuation[2] * distance * distance);
        } else {
            // Directional light
            lightDir = normalize(lightPos);
        }

        bool inShadow = traceShadowRay(intersection.point, lightPos, scene);

        float visibility = inShadow ? 0.0f : 1.0f;

        float diffuseIntensity = max(0.0f, dot(intersection.normal, lightDir));
        diffuse += visibility * vec3(obj.diffuse[0], obj.diffuse[1], obj.diffuse[2]) * lightColor * diffuseIntensity * attenuation;

        vec3 viewDir = normalize(cameraEye - intersection.point);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float specularIntensity = pow(max(0.0f, dot(intersection.normal, halfwayDir)), obj.shininess);
        specular += visibility * vec3(obj.specular[0], obj.specular[1], obj.specular[2]) * lightColor * specularIntensity * attenuation;
    }

   
    if (depth < scene.maxDepth && (obj.specular[0] > 0.0f || obj.specular[1] > 0.0f || obj.specular[2] > 0.0f)) {
        vec3 reflectDir = normalize(intersection.ray.direction - 2.0f * dot(intersection.ray.direction, intersection.normal) * intersection.normal);
        Ray reflectRay(intersection.point + reflectDir * 0.001f, reflectDir);

        Intersection reflectHit;
        for (int i = 0; i < numobjects; i++) {
            const object& reflectObj = objects[i];
            Intersection hit;
            vec3 sphereCenter;
            switch (reflectObj.type) {
                case triangle:
                    hit = intersectTriangle(reflectRay, vertices[reflectObj.coords[0]], vertices[reflectObj.coords[1]], vertices[reflectObj.coords[2]], reflectObj.transform);
                    break;
                case sphere:
                    sphereCenter = vec3(reflectObj.coords[0], reflectObj.coords[1], reflectObj.coords[2]);
                    hit = intersectSphere(reflectRay, sphereCenter, reflectObj.size, reflectObj.transform);
                    break;
                default:
                    break;
            }
            if (hit.hit && hit.distance < reflectHit.distance) {
                reflectHit = hit;
                reflectHit.objectIndex = i;
            }
        }

        if (reflectHit.hit) {
            const object& reflectObj = objects[reflectHit.objectIndex];
            vec3 reflectColor = calculateShading(reflectHit, reflectObj, cameraEye, scene, depth + 1);
            specular += vec3(obj.specular[0], obj.specular[1], obj.specular[2]) * reflectColor;
        }
    }

    return ambient + emission + diffuse + specular;
}

void saveScreenshot(string fname) {
  int pix = w * h;
  BYTE *pixels = new BYTE[3*pix];	
  glReadBuffer(GL_FRONT);
  glReadPixels(0,0,w,h,GL_BGR,GL_UNSIGNED_BYTE, pixels);

  FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

  std::cout << "Saving screenshot: " << fname << "\n";

  FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
  delete[] pixels;
}

void printSceneInfo() {
    cout << "\nLight Information:\n";
for (int i = 0; i < numused; i++) {
    cout << "Light " << i + 1 << ":\n";
    cout << " Position: " 
         << lightposn[i * 4] << " " 
         << lightposn[i * 4 + 1] << " " 
         << lightposn[i * 4 + 2] << " " 
         << lightposn[i * 4 + 3] << endl;
    cout << " Color: " 
         << lightcolor[i * 4] << " " 
         << lightcolor[i * 4 + 1] << " " 
         << lightcolor[i * 4 + 2] << " " 
         << lightcolor[i * 4 + 3] << endl;
    cout << "------------------------\n";
}


    cout << "\nCamera Settings:\n";
    cout << "Eye Position: " << eyeinit.x << ", " << eyeinit.y << ", " << eyeinit.z << endl;
    cout << "Center Point: " << center.x << ", " << center.y << ", " << center.z << endl;
    cout << "Up Vector: " << upinit.x << ", " << upinit.y << ", " << upinit.z << endl;
    cout << "Field of View (Y): " << fovy << endl;

    

    cout << "\nObjects Information:\n";
    for (int i = 0; i < numobjects; i++) {
        cout << "Object " << i + 1 << ":\n";
        cout << " Type: ";
        switch(objects[i].type) {
            case triangle: cout << "Triangle"; break;
            case cube: cout << "Cube"; break;
            case sphere: cout << "Sphere"; break;
            case teapot: cout << "Teapot"; break;
            default: cout << "Unknown";
        }
        cout << "\n Size: " << objects[i].size << endl;
        cout << " Ambient: " << objects[i].ambient[0] << " " << objects[i].ambient[1] << " " << objects[i].ambient[2] << " " << objects[i].ambient[3] << endl;
        cout << " Diffuse: " << objects[i].diffuse[0] << " " << objects[i].diffuse[1] << " " << objects[i].diffuse[2] << " " << objects[i].diffuse[3] << endl;
        cout << " Specular: " << objects[i].specular[0] << " " << objects[i].specular[1] << " " << objects[i].specular[2] << " " << objects[i].specular[3] << endl;
        cout << " Emission: " << objects[i].emission[0] << " " << objects[i].emission[1] << " " << objects[i].emission[2] << " " << objects[i].emission[3] << endl;
        cout << " Shininess: " << objects[i].shininess << endl;
        cout << " Coordinates/Position: ";
        for (int j = 0; j < 4; j++) {
            cout << objects[i].coords[j] << " ";
        }
        cout << "\nTransform Matrix: \n"; 
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                cout << objects[i].transform[j][k] << " ";
            }
            cout << endl;
        }
        cout << "------------------------\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "incorrect usage"; 
        exit(-1); 
    }

    FreeImage_Initialise();
    glutInit(&argc, argv);

    // OSX systems require an extra window init flag
    #ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    #else
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    #endif
    glutCreateWindow("HW4");

    #ifndef __APPLE__ // GLew not needed on OSX systems
    GLenum err = glewInit(); 
    if (GLEW_OK != err) { 
        std::cerr << "Error: " << glewGetString(err) << std::endl; 
    } 
    #endif
    attenuation = vec3(1.0f, 0.0f, 0.0f);
    maxdepth = 5;
    readfile(argv[1]);
    printSceneInfo();

    Camera camera(eyeinit, center, upinit, fovy, w, h);
    Image image(w, h);
    Scene scene;
    scene.camera = Camera(eyeinit, center, upinit, fovy, w, h);
    scene.attenuation = attenuation;
    scene.maxDepth = maxdepth;
    rayTrace(camera, image, scene);

   
    FIBITMAP* img = FreeImage_Allocate(image.width, image.height, 24);
    RGBQUAD color;
    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < image.width; x++) {
            vec3 pixelColor = image.getPixel(x, y);
            color.rgbBlue = static_cast<BYTE>(pixelColor.x * 255);
            color.rgbGreen = static_cast<BYTE>(pixelColor.y * 255);
            color.rgbRed = static_cast<BYTE>(pixelColor.z * 255);
            FreeImage_SetPixelColor(img, x, y, &color);
        }
    }

    string inputFilename(argv[1]);
    string outputFilename = inputFilename + ".png";
    FreeImage_Save(FIF_PNG, img, outputFilename.c_str(), 0);
    FreeImage_Unload(img);

    FreeImage_DeInitialise();

    return 0;
}


