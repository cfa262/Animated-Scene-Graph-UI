#include "poly.h"

poly::poly() {
	this->vertices = new std::list<vec3>();
	this->color[0] = 1.0;
	this->color[1] = 1.0;
	this->color[2] = 1.0;
}
void poly::setColor(float red, float green, float blue)
{	
	this->color[0] = red;
	this->color[1] = green;
	this->color[2] = blue;
}

void poly::draw(mat3 transformation) {
	int count = 0;
	glColor3f(this->color[0], this->color[1], this->color[2]);
	for (std::list<vec3>::const_iterator it = (this->vertices)->begin(); it !=
		(this->vertices)->end(); ++it)
	{
		count++;
	}
	if (count == 3) {
		glBegin(GL_TRIANGLES);
	}
	else if (count == 4) {
		glBegin(GL_QUADS);
	}
	else if (count > 4 && count < 6) {
		glBegin(GL_POLYGON);
	}
	else {
		glBegin(GL_LINE_LOOP);
	}

	for (std::list<vec3>::const_iterator it = (this->vertices)->begin(); it !=
		(this->vertices)->end(); ++it)
	{
		vec3 vertex = transformation * (*it);
		glVertex2f(vertex[0], vertex[1]);
	}
	glEnd();
}
void poly::draw() {
	int count=0;
	glColor3f(this->color[0], this->color[1], this->color[2]);
	for (std::list<vec3>::const_iterator it = (this->vertices)->begin(); it !=
		(this->vertices)->end(); ++it)
	{
		count++;
	}
	if (count == 3) {
		glBegin(GL_TRIANGLES);
	}
	else if (count == 4) {
		glBegin(GL_QUADS);
	}
	else if (count > 4 && count < 7) {
		glBegin(GL_POLYGON);
	}
	else {
		glBegin(GL_LINE_LOOP);
	}

	for (std::list<vec3>::const_iterator it = (this->vertices)->begin(); it !=
		(this->vertices)->end(); ++it)
	{
		vec3 vertex = *it;
		glVertex2f(vertex[0], vertex[1]);
	}
	glEnd();

}

const vec3 poly::getColor()
{
	return this->color;
}

const std::list<vec3>* poly::getVertices()
{
	return this->vertices;
}

void poly::setVertices(std::list<vec3>* v)
{
	this->vertices = v;
}



















