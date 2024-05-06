// Transform.cpp: implementation of the Transform class.


#include <stdio.h>
#include "Transform.h"

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
	const vec3 unitAxis = normalize(axis);
    const float rads = (pi*degrees)/ 180.0f;
    const float x = unitAxis.x;
    const float y = unitAxis.y;
    const float z = unitAxis.z;
	const mat3 I = mat3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
    const mat3 aaT = mat3(x*x, x*y, x*z, y*x, y*y, y*z, z*x, z*y, z*z);
    const mat3 As = mat3(0, -z, y, z, 0, -x, -y, x, 0);
    return  I * cosf(rads)  + aaT * (1 - cosf(rads)) + As * sinf(rads);
}


// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE
	const mat3 rotated = rotate(-degrees,up);
	eye = rotated * eye;
	printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));

}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE 
	const mat3 rotated = rotate(degrees,cross(up,eye));
	eye = rotated * eye;
	up = rotated * up;
	printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));


}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
	// YOUR CODE FOR HW1 HERE
	vec3 w = normalize(eye);
	vec3 u = normalize(cross(up, w));
	vec3 v = cross(w,u);



	// You will change this return call
	return mat4( 
		u.x, v.x, w.x, 0,
		u.y, v.y, w.y, 0,
		u.z, v.z, w.z, 0, 
		-u.x * eye.x - u.y * eye.y - u.z * eye.z,
		-v.x * eye.x - v.y * eye.y - v.z * eye.z,
		-w.x * eye.x - w.y * eye.y - w.z * eye.z, 
		1
	 );
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
