#pragma once

#include "mat3.h"
#include "vec3.h"
#include "myglwidget.h"
#include <list>

class poly
{
private:
	vec3 color;
	std::list<vec3>* vertices;

public:
	poly();
	void setColor(float red, float green, float blue);
	//void transform();
	void draw(mat3 transformation);
	void draw();
	const vec3 getColor();
	const std::list<vec3>* getVertices();
	void setVertices(std::list<vec3>* v);
};


