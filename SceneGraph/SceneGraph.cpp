#include "SceneGraph.h"

SceneGraph::SceneGraph()
{
	this->root = new Node();
}
//makes a copy of the scene graph also copying the root
SceneGraph::SceneGraph(const SceneGraph &sg) {
	this->root = new Node();
	*root = *sg.root;
}
void SceneGraph::setRoot(Node* r) {
	this->root = r;
}
Node* SceneGraph::getRoot()
{
	return this->root;
}
SceneGraph::~SceneGraph() {
	delete root;
	this->root = new Node();
}
void SceneGraph::addKid(Node* parent, Node* child) {
	parent->addChild(child);
	//->setParent(parent);
}

void SceneGraph::traverse(Node* node, mat3 transformation)
{
	if (node != NULL) {

		transformation = transformation * node->finalTransformation;

		if ((node->isPoly) == true)
		{
			node->shape->draw(transformation);
		}

		for (std::list<Node*>::const_iterator it = (node->getChildren())->begin();
			it != (node->getChildren())->end(); ++it)
		{
			
			traverse((*it), transformation);
		}
	}
}
//
//void SceneGraph::transform(Node* parent, Node* n) 
//{
//	Node* b = new Node();
//	b->shape = n->Node::multiply(parent->rotation);
//	b->shape = n->Node::multiply(parent->scl);
//	b->shape = n->Node::multiply(parent->translation);
//	if (n->getParent() != NULL) {
//		transform(n->parent, b);
//	}
//}