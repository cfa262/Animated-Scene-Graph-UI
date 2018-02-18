#pragma once

#include <list>
#include <vector>
#include "mat3.h"
#include "vec3.h"
#include "poly.h"

class Node {
	
	
private:
	std::list<Node*>* children;	//list of pointers for children Nodes
	
public:
	float scaleX; //1
	float scaleY; //1
	float transX; //1
	float transY; //1
	float rot;
	std::string name;
	Node(std::string s);
	Node();
	~Node();
	Node* parent; //pointer to parent node
	poly* shape; //polygon variable
	bool isPoly = false;

	//transformation matrices
	mat3 translation;
	mat3 scl;
	mat3 rotation;
	mat3 finalTransformation = mat3::identity();

	void setPolyTrue();
	void transform();
	std::list<Node*>* getChildren();
	void setChildren(std::list<Node*>* c);
	Node* getParent();
	void setParent(Node* p);
	poly* multiply(mat3 matrix);
	friend bool operator==(Node & n1, Node & n2);

	void rotate(float angle);
	void scale(float x, float y);
	void translate(float x, float y);
	void translate();
	void scale();
	void rotate();

	void addChild(Node* n);
	void removeNode(Node * r);
	const poly* getShape();
	void setShape(poly * newShape);
	void setTransformation(mat3 scale, mat3 rotate, mat3 translate);
	mat3 getTransformation();

};
