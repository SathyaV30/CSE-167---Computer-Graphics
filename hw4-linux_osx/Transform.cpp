// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"
#include "stdio.h"
// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
	  const vec3 unitAxis = normalize(axis);
    const float rads = (pi*degrees)/ 180.0f;
    const float x = unitAxis.x;
    const float y = unitAxis.y;
    const float z = unitAxis.z;
	  const mat3 I = mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);

    const mat3 aaT = mat3(x*x, x*y, x*z, y*x, y*y, y*z, z*x, z*y, z*z);
    const mat3 As = mat3(0, z, -y, -z, 0, x, y, -x, 0);
    return  I * cosf(rads)  + aaT * (1 - cosf(rads)) + As * sinf(rads);
}



// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE
	const mat3 rotated = rotate(degrees,up);
	eye = rotated * eye;
	printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));

}
// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
	// YOUR CODE FOR HW1 HERE 
	const mat3 rotated = rotate(-degrees,cross(up,eye));
	eye = rotated * eye;
	up = rotated * up;

	printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));


}


mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
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

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  // YOUR CODE FOR HW2 HERE
  // New, to implement the perspective transform as well.  
  float theta = (pi*fovy)/360.f;
  float d = 1/tanf(theta);
  float A = -(zFar + zNear)/(zFar - zNear);
  float B = -(2 * zFar * zNear)/(zFar - zNear);
  return 
  mat4
  (d/aspect, 0, 0, 0,
   0, d, 0, 0, 
   0, 0, A, -1, 
   0, 0, B, 0);
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  return 
  mat4(
  sx, 0, 0, 0, 
  0, sy, 0, 0, 
  0, 0, sz, 0, 
  0, 0, 0, 1);
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) {
  return 
  mat4(
  1, 0, 0, 0, 
  0, 1, 0, 0,
  0, 0, 1, 0,
  tx, ty, tz, 1   
  );

}


// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
