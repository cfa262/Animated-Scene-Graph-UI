
#include "vec3.h"
#include <assert.h>
#include <iostream>
#include <cmath>
#define EPSILON 0.0000001


vec3::vec3() {
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
}
vec3::vec3(float x, float y, float z) {
	data[0] = x;
	data[1] = y;
	data[2] = z;
}

float vec3::operator[](unsigned int index) const{
	assert(index >= 0 && index < 3);
	return (this->data)[index];
}
float& vec3::operator[](unsigned int index) {
	assert(index >= 0 && index < 3);
	return data[index];
}

void vec3::print() {
	using namespace std;
	cout << "(" << data[0] << ", " << data[1] << ", " << data[2] << ")" << endl; 
}
float vec3::length() const {  //returns the geometric length of vector
	float len;
	len = sqrt(pow(data[0], 2.0) + pow(data[1], 2) + pow(data[2], 2)); //square root of(x^2 + y^2 + z^2)
	return len;
}
//checks equality of two vectors
bool operator==(const vec3& v1, const vec3& v2) {
	if (v1.data[0] == v2.data[0] && v1.data[1] == v2.data[1] && v1.data[2] == v2.data[2]) {
			return true; 
	}
	else {
		return false;
	}
}
//checks for non equality of two vectors
bool operator!=(const vec3& v1, const vec3& v2) {
	if (v1.data[0] == v2.data[0] && v1.data[1] == v2.data[1] && v1.data[2] == v2.data[2]) {
		return false;
	}
	else {
		return true;
	}
}
//addition of two vectors
vec3 operator+(const vec3& v1, const vec3& v2) {
	vec3 vec;
	vec.data[0] = v1.data[0] + v2.data[0];
	vec.data[1] = v1.data[1] + v2.data[1];
	vec.data[2] = v1.data[2] + v2.data[2];
	return vec;
}
//subtraction of two vectors 
vec3 operator-(const vec3& v1, const vec3& v2) {
	vec3 vec;
	vec.data[0] = v1.data[0] - v2.data[0];
	vec.data[1] = v1.data[1] - v2.data[1];
	vec.data[2] = v1.data[2] - v2.data[2];
	return vec;
}
//multiplication of scalar vectors
vec3 operator*(const vec3& v,  float c) {
	vec3 vec;
	vec.data[0] = v.data[0] * c;
	vec.data[1] = v.data[1] * c;
	vec.data[2] = v.data[2] * c;
	return vec;
}
vec3 operator*(float c, const vec3& v) {
	vec3 vec;
	vec.data[0] = c * v.data[0];
	vec.data[1] = c * v.data[1];
	vec.data[2] = c * v.data[2];
	return vec;
}

//division of vector with a number
vec3 operator/(const vec3& v, float c) {
	vec3 vec;
	vec.data[0] = v.data[0] / c;
	vec.data[1] = v.data[1] / c;
	vec.data[2] = v.data[2] / c;
	return vec;
}

//dot product of two vectors 
float operator*(const vec3& v1, const vec3& v2) {
	float mult;
	float temp;
	mult = v1.data[0] * v2.data[0];
	temp = v1.data[1] * v2.data[1];
	mult = mult + temp;
	temp = v1.data[2] * v2.data[2];
	return mult + temp;
}

//cross product of two vectors
vec3 operator%(const vec3& v1, const vec3& v2) {
	vec3 cross;
	cross.data[0] = (v1.data[1] * v2.data[2]) - (v2.data[1] * v1.data[2]);
	cross.data[1] = -1 * ((v1.data[0] * v2.data[2]) - (v2.data[0] * v1.data[2]));
	cross.data[2] = (v1.data[0] * v2.data[1] - v2.data[0] * v1.data[1]);
	return cross;
}




