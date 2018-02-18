#include "mat3.h"  
#include <iostream>
#include <assert.h>
#include "vec3.h"
#define EPSILON 0.0000001
#define PI 3.14159265


mat3::mat3() {
	data[0] = vec3();
	data[1] = vec3();
	data[2] = vec3();
}
mat3::mat3(const vec3& row1, const vec3& row2, const vec3& row3) {
	data[0] = row1;
	data[1] = row2;
	data[2] = row3;
}

vec3 mat3::operator[](unsigned int index)const {
	assert(index >= 0 && index < 3);
	return data[index];
}
vec3& mat3::operator[](unsigned int index) {
	assert(index >= 0 && index < 3);
	return data[index];
}
vec3 mat3::getColumn(unsigned int index)const {
	assert(index >= 0 && index < 3);
	vec3 column;
	vec3 temp;
	/*temp = data[0];
	column[0] = temp[index];
	temp = data[1];
	column[1] = temp[index];
	temp = data[2];
	column[2] = temp[index];
	return column;*/

	return vec3(data[0][index], data[1][index], data[2][index]);
}

void mat3::print() {
	vec3 v1 = data[0];
	vec3 v2 = data[1];
	vec3 v3 = data[2];
	v1.print();
	v2.print();
	v3.print();
}

mat3 mat3::transpose() const {
	mat3 transpose;
	transpose.data[0] = getColumn(0);
	transpose.data[1] = getColumn(1);
	transpose.data[2] = getColumn(2);
	return transpose;
}

mat3 mat3::rotation2D(float angle) {
	mat3 rotate;
	float deg = 180;
	angle = (angle* PI / deg);
	rotate.data[0] = vec3(cos(angle), -sin(angle), 0);
	rotate.data[1] = vec3(sin(angle), cos(angle), 0);
	rotate.data[2] = vec3(0, 0, 1);
	return rotate;
}
mat3 mat3::translation2D(float x, float y) {
	mat3 trans;
	trans.data[0] = vec3(1, 0, x);
	trans.data[1] = vec3(0, 1, y);
	trans.data[2] = vec3(0, 0, 1);
	return trans;
}
mat3 mat3::scale2D(float x, float y) {
	mat3 scale;
	scale.data[0] = vec3(x, 0, 0);
	scale.data[1] = vec3(0, y, 0);
	scale.data[2] = vec3(0, 0, 1);
	return scale;
}
mat3 mat3::identity() {
	mat3 id;
	id.data[0] = vec3(1, 0, 0);
	id.data[1] = vec3(0, 1, 0);
	id.data[2] = vec3(0, 0, 1);
	return id;
}
bool operator==(const mat3& m1, const mat3& m2) {
	vec3 r1 = m1.data[0];
	vec3 r2 = m1.data[1];
	vec3 r3 = m1.data[2];
	vec3 row1 = m2.data[0];
	vec3 row2 = m2.data[1];
	vec3 row3 = m2.data[2];

	if (r1 == row1 && r2 == row2 && r3 == row3) {
		return true;
	}
	else {
		return false;
	}
}
bool operator!=(const mat3& m1, const mat3& m2) {
	vec3 r1 = m1.data[0];
	vec3 r2 = m1.data[1];
	vec3 r3 = m1.data[2];
	vec3 row1 = m2.data[0];
	vec3 row2 = m2.data[1];
	vec3 row3 = m2.data[2];

	if (r1 != row1 && r2 != row2 && r3 != row3) {
		return true;
	}
	else {
		return false;
	}
}
mat3 operator+(const mat3& m1, const mat3& m2) {
	vec3 v1 = m1.data[0];
	vec3 v2 = m1.data[1];
	vec3 v3 = m1.data[2];
	vec3 vector1 = m2.data[0];
	vec3 vector2 = m2.data[1];
	vec3 vector3 = m2.data[2];

	mat3 addMat;
	addMat.data[0] = v1 + vector1;
	addMat.data[1] = v2 + vector2;
	addMat.data[2] = v3 + vector3;
	return addMat;
}

mat3 operator-(const mat3& m1, const mat3& m2) {
	vec3 v1 = m1.data[0];
	vec3 v2 = m1.data[1];
	vec3 v3 = m1.data[2];
	vec3 vector1 = m2.data[0];
	vec3 vector2 = m2.data[1];
	vec3 vector3 = m2.data[2];

	mat3 subMat;
	subMat.data[0] = v1 - vector1;
	subMat.data[1] = v2 - vector2;
	subMat.data[2] = v3 - vector3;
	return subMat;
}

mat3 operator*(const mat3& m, float c) {
	vec3 v1 = m.data[0];
	vec3 v2 = m.data[1];
	vec3 v3 = m.data[2];

	mat3 multMat;
	multMat.data[0] = v1 * c;
	multMat.data[1] = v2 * c;
	multMat.data[2] = v3 * c;
	return multMat;
}
mat3 operator*(float c, const mat3& m) {
	vec3 v1 = m.data[0];
	vec3 v2 = m.data[1];
	vec3 v3 = m.data[2];

	mat3 multMat;
	multMat.data[0] = c * v1;
	multMat.data[1] = c * v2;
	multMat.data[2] = c * v3;
	return multMat;
}
mat3 operator/(const mat3& m, float c) {
	vec3 v1 = m.data[0];
	vec3 v2 = m.data[1];
	vec3 v3 = m.data[2];

	mat3 divMat;
	divMat.data[0] = v1 / c;
	divMat.data[1] = v2 / c;
	divMat.data[2] = v3 / c;
	return divMat;
}

//matrix multiplication
mat3 operator*(const mat3& m1, const mat3& m2) {

	vec3 row1 = vec3(m1[0] * m2.getColumn(0),
					 m1[0] * m2.getColumn(1),
					 m1[0] * m2.getColumn(2));

	vec3 row2 = vec3(m1[1] * m2.getColumn(0),
					 m1[1] * m2.getColumn(1),
					 m1[1] * m2.getColumn(2));

	vec3 row3 = vec3(m1[2] * m2.getColumn(0),
					 m1[2] * m2.getColumn(1),
					 m1[2] * m2.getColumn(2));

	// Return a new matrix of the new vectors 
	return mat3(row1,row2,row3);
	
}
// MATRIX/VECTOR mulitplication
// Assume v is a column vector
vec3 operator*(const mat3& m, const vec3& v) {
	vec3 v1 = m.data[0];
	vec3 v2 = m.data[1];
	vec3 v3 = m.data[2];

	vec3 vector;
	float num;
	num = v1[0] * v[0] + v1[1] * v[1] + v1[2] * v[2];
	vector[0] = num;
	num = v2[0] * v[0] + v2[1] * v[1] + v2[2] * v[2];
	vector[1] = num;
	num = v3[0] * v[0] + v3[1] * v[1] + v3[2] * v[2];
	vector[2] = num;
	return vector;
}
// VECTOR/MATRIX Multiplication
// Assume v is a column vector
vec3 operator*(const vec3& v, const mat3& m) {
	vec3 v1 = m.data[0];
	vec3 v2 = m.data[1];
	vec3 v3 = m.data[2];

	vec3 vector;
	float num;
	num = v1[0] * v[0] + v2[1] * v[1] + v3[2] * v[2];
	vector[0] = num;
	num = v1[0] * v[0] + v2[1] * v[1] + v3[2] * v[2];
	vector[1] = num;
	num = v1[0] * v[0] + v2[1] * v[1] + v3[2] * v[2];
	vector[2] = num;
	return vector;

}





