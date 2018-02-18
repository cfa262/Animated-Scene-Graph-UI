#include "node.h"


Node::~Node() {
	//delete this->parent;
	//delete this->shape;
	delete this->children;
}
Node::Node(std::string s) {
	this->name = s;
	this->parent = NULL;
	this->scaleX = 1.0;
	this->scaleY = 1.0;
	this->rot = 0.0;
	this->shape = new poly();
	this->children = new std::list<Node*>();
	this->translation = mat3::identity();
	this->rotation = mat3::identity();
	this->scl = mat3::identity();
}
Node::Node() {
	this->parent = NULL;
	/*this->transX = 1.0;
	this->transY = 1.0;
	this->scaleX = 1.0;
	this->scaleY = 1.0;
	this->rot = 0.0;*/
	this->shape = new poly();
	this->children = new std::list<Node*>();
	this->translation = mat3::identity();
	this->rotation = mat3::identity();
	this->scl = mat3::identity();
}

void Node::setPolyTrue() {
	this->isPoly = true;
}
void Node::transform() {
	this->finalTransformation = this->translation*this->rotation*this->scl;
}
mat3 Node::getTransformation() {
	return this->finalTransformation;
}
poly* Node::multiply(mat3 matrix)
{
	std::list<vec3>* newList = new std::list<vec3>;

	for (std::list<vec3>::const_iterator p = (this->shape->getVertices())->cbegin();
		p != ((this->shape->getVertices())->end()); ++p)
	{
		vec3 vector = matrix * (*p);
		newList->push_back(vector);
	}
	this->shape->setVertices(newList);
	return this->shape;
}

void Node::setTransformation(mat3 scale, mat3 rotate, mat3 translate)
{
	this->translation = translate;
	this->scl = scale;
	this->rotation = rotate;
	this->finalTransformation = translate*rotate*scale;
}
void Node::rotate() {
	this->rotation = this->rotation * (mat3::rotation2D(this->rot));
}
void Node::rotate(float angle) {
	this->rotation = this->rotation * (mat3::rotation2D(angle));
}
void Node::translate() {
	this->translation = this->translation * (mat3::translation2D(this->transX, this->transY));
}
void Node::translate(float x, float y) {
	this->translation = this->translation * (mat3::translation2D(x, y));
}
void Node::scale() {
	this->scl = this->scl * (mat3::scale2D(this->scaleX, this->scaleY));
}
void Node::scale(float x, float y) {
	this->scl = this->scl * (mat3::scale2D(x, y));
}
void Node::addChild(Node * child)
{
	if (child != NULL) 
	{
         child->parent = this;
	     this->children->push_back(child);
	}
}
void Node::removeNode(Node * rNode)
{
	for (std::list<Node*>::iterator it = this->children->begin(); it !=
		this->children->end(); ++it)
	{
		if ((*it) == rNode)
		{
			(this->children)->erase(it);
			return;
		}
	}
}
bool operator ==(Node& n1, Node& n2)
{
	if (n1.shape != n2.shape)
	{
		return false;
	}

	if (n1.finalTransformation != n2.finalTransformation)
	{
		return false;
	}

	return true;
}

const poly * Node::getShape(){return shape;}

void Node::setShape(poly * newShape){this->shape = newShape;}

std::list<Node*>* Node::getChildren(){return this->children;}

void Node::setChildren(std::list<Node*>* c) {
	this->children = c;
}

Node* Node::getParent(){return this->parent;}

void Node::setParent(Node * p){this->parent = p;}


