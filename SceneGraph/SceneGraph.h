#pragma once

#include "mat3.h"
#include "vec3.h"
#include "node.h"
//#include "poly.h"
#include <list>

class SceneGraph{

private:

public:
	Node* root;
	SceneGraph();
	SceneGraph(const SceneGraph & sg);
	void setRoot(Node * r);
	~SceneGraph();
	Node* getRoot();
	void addKid(Node* parent, Node* kid);
	void traverse(Node * node, mat3 transformation);
	//void traverse(Node * root);
};
