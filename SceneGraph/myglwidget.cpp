// Implementation of the MyGLWidget class
// Your scene graph will be created and called from here and OpenGL calls to display it. 

#include <QtWidgets>
#include <QtOpenGL>  // Note that some of the OpenGL calls are Qt specific and not standard OpenGL

#include "myglwidget.h"
#include "window.h"
#include "SceneGraph.h"
#include "node.h"

MyGLWidget::MyGLWidget(QWidget *parent)					 // Constructor 
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)   // Call to parent constructor

{
	// You might create your scene graph and animal directly in the constructor 
	// or create another method for doing the creating and call it from here
	this->glScene = new SceneGraph();
	//this->sceneRoot = new Node();
}

MyGLWidget::~MyGLWidget()	// Destructor
{
	// Normally you destroy what you created, but I'm not worried about it just now. 
}

////////////////////////////////////////////////////////////////////////////
// A few more Qt methods that you don't need to worry about yet. 
QSize MyGLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360)
        angle -= 360 * 16;
}
///////////////////////////////////////////////////////////////////////////////

// Initializing OpenGL
void MyGLWidget::initializeGL()
{
	qglClearColor(Qt::darkBlue);	// Set the background color. Note this isn't standard OpenGL. It's Qt. 

	// We'll add more code here later in the semester
}

// This method gets called whenever the widget needs updating. 
void MyGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT); // Clear the window using the background color set in initialization
 
    draw();
}

// YOUR CODE GOES IN THIS DRAW METHOD
// Actual code for drawing the primitives
void MyGLWidget::draw()
{
	// YOUR CODE MIGHT CALL YOUR SCENE GRAPH TRAVERSAL METHOD
	// WHICH WOULD IN TURN APPLY TRANSFORMS TO SHAPES
	// AND ULTIMATELY CALL OPENGL COMMANDS

	/*SceneGraph* s = new SceneGraph();
	if (sceneRoot != NULL) {
		s->traverse(this->sceneRoot, mat3::identity());
	}*/

	if (this->glScene != NULL) {
		this->glScene->traverse(this->glScene->root, mat3::identity());
	}
}

/////////////////////////////////////////////////////////////////
// More Qt methods that you do not have to worry about just now
void MyGLWidget::resizeGL(int width, int height)
{

	glViewport(0, 0, this->width(), this->height());  // lower left coordinate and width and height

										//glOrtho describes a transformation that produces a parallel projection.
	glOrtho(-10, 10, -10, 10, -1, 1);   // left, right, bottom, top, near, far

}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
   
}
//void MyGLWidget::restartAnimal() {
//	SceneGraph* scene = new SceneGraph();
//
//	Node* torso = new Node();
//	torso->setPolyTrue();
//	torso->shape->setColor(.75, .75, .5);
//	//mat3 scale = mat3::scale2D(2, 2);
//	//mat3 rot = mat3::rotation2D(45);
//	torso->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(1, 1));
//	//torso->transform();
//	std::list <vec3> *v = new std::list<vec3>();
//	v->push_back(vec3(-4, -2, 1));
//	v->push_back(vec3(-4, 2, 1));
//	v->push_back(vec3(4, 2, 1));
//	v->push_back(vec3(4, -2, 1));
//	torso->shape->setVertices(v);
//
//	Node* head = new Node();
//	head->setPolyTrue();
//	head->shape->setColor(.4, 1, .8);
//	head->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(6, 3));
//	std::list <vec3> *h = new std::list<vec3>();
//	h->push_back(vec3(-2, -1, 1));
//	h->push_back(vec3(-2, 2, 1));
//	h->push_back(vec3(0, 4, 1));
//	h->push_back(vec3(2, 2, 1));
//	h->push_back(vec3(2, -1, 1));
//	head->shape->setVertices(h);
//
//	Node* leftEar = new Node();
//	leftEar->setPolyTrue();
//	leftEar->shape->setColor(1, .4, .6);
//	leftEar->setTransformation(mat3::scale2D(.75, .75), mat3::identity(), mat3::translation2D(-1, 2));
//	std::list <vec3> *e = new std::list<vec3>();
//	e->push_back(vec3(-1, 0, 1));
//	e->push_back(vec3(-1, 2, 1));
//	e->push_back(vec3(1, 2, 1));
//	leftEar->shape->setVertices(e);
//
//	Node* rightEar = new Node();
//	rightEar->setPolyTrue();
//	rightEar->shape->setColor(1, .4, .6);
//	rightEar->setTransformation(mat3::scale2D(.75, .75), mat3::identity(), mat3::translation2D(1, 2));
//	std::list <vec3> *r = new std::list<vec3>();
//	r->push_back(vec3(1, 0, 1));
//	r->push_back(vec3(-1, 2, 1));
//	r->push_back(vec3(1, 2, 1));
//	rightEar->shape->setVertices(r);
//
//	Node* leftLeg = new Node();
//	leftLeg->setPolyTrue();
//	leftLeg->shape->setColor(.25, .25, 1);
//	leftLeg->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(-3.5, -4));
//	std::list <vec3> *lLeg = new std::list<vec3>();
//	lLeg->push_back(vec3(-.5, -2, 1));
//	lLeg->push_back(vec3(-.5, 2, 1));
//	lLeg->push_back(vec3(.5, 2, 1));
//	lLeg->push_back(vec3(.5, -2, 1));
//	leftLeg->shape->setVertices(lLeg);
//
//	Node* rightLeg = new Node();
//	rightLeg->setPolyTrue();
//	rightLeg->shape->setColor(.25, .25, 1);
//	rightLeg->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(-1, -4));
//	std::list <vec3> *rLeg = new std::list<vec3>();
//	rLeg->push_back(vec3(-.5, -2, 1));
//	rLeg->push_back(vec3(-.5, 2, 1));
//	rLeg->push_back(vec3(.5, 2, 1));
//	rLeg->push_back(vec3(.5, -2, 1));
//	rightLeg->shape->setVertices(rLeg);
//
//	Node* leftArm = new Node();
//	leftArm->setPolyTrue();
//	leftArm->shape->setColor(.25, .25, 1);
//	leftArm->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(1, -4));
//	std::list <vec3> *lArm = new std::list<vec3>();
//	lArm->push_back(vec3(-.5, -2, 1));
//	lArm->push_back(vec3(-.5, 2, 1));
//	lArm->push_back(vec3(.5, 2, 1));
//	lArm->push_back(vec3(.5, -2, 1));
//	leftArm->shape->setVertices(lArm);
//
//	Node* rightArm = new Node();
//	rightArm->setPolyTrue();
//	rightArm->shape->setColor(.25, .25, 1);
//	rightArm->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(3.5, -4));
//	std::list <vec3> *rArm = new std::list<vec3>();
//	rArm->push_back(vec3(-.5, -2, 1));
//	rArm->push_back(vec3(-.5, 2, 1));
//	rArm->push_back(vec3(.5, 2, 1));
//	rArm->push_back(vec3(.5, -2, 1));
//	rightArm->shape->setVertices(rArm);
//
//	Node* leftFoot = new Node();
//	leftFoot->setPolyTrue();
//	leftFoot->shape->setColor(.25, .5, .75);
//	leftFoot->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(.5, -3));
//	std::list <vec3> *lFoot = new std::list<vec3>();
//	lFoot->push_back(vec3(-1, 0, 1));
//	lFoot->push_back(vec3(-1, 1, 1));
//	lFoot->push_back(vec3(1, 1, 1));
//	lFoot->push_back(vec3(1, 0, 1));
//	leftFoot->shape->setVertices(lFoot);
//
//	Node* rightFoot = new Node();
//	rightFoot->setPolyTrue();
//	rightFoot->shape->setColor(1, .25, 1);
//	rightFoot->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(.5, -3));
//	std::list <vec3> *rFoot = new std::list<vec3>();
//	rFoot->push_back(vec3(-1, 0, 1));
//	rFoot->push_back(vec3(-1, 1, 1));
//	rFoot->push_back(vec3(1, 1, 1));
//	rFoot->push_back(vec3(1, 0, 1));
//	rightFoot->shape->setVertices(rFoot);
//
//	Node* leftHand = new Node();
//	leftHand->setPolyTrue();
//	leftHand->shape->setColor(1, 1, .25);
//	leftHand->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(.5, -3));
//	std::list <vec3> *lHand = new std::list<vec3>();
//	lHand->push_back(vec3(-1, 0, 1));
//	lHand->push_back(vec3(-1, 1, 1));
//	lHand->push_back(vec3(1, 1, 1));
//	lHand->push_back(vec3(1, 0, 1));
//	leftHand->shape->setVertices(lHand);
//
//	Node* rightHand = new Node();
//	rightHand->setPolyTrue();
//	rightHand->shape->setColor(1, .5, 0);
//	rightHand->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(.5, -3));
//	std::list <vec3> *rHand = new std::list<vec3>();
//	rHand->push_back(vec3(-1, 0, 1));
//	rHand->push_back(vec3(-1, 1, 1));
//	rHand->push_back(vec3(1, 1, 1));
//	rHand->push_back(vec3(1, 0, 1));
//	rightHand->shape->setVertices(rHand);
//
//	Node* tail = new Node();
//	tail->setPolyTrue();
//	tail->shape->setColor(.5, .5, .5);
//	tail->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(-5, 1));
//	std::list <vec3> *t = new std::list<vec3>();
//	t->push_back(vec3(-2, 0, 1));
//	t->push_back(vec3(-2, 1, 1));
//	t->push_back(vec3(-.5, 2, 1));
//	t->push_back(vec3(-.5, 1, 1));
//	t->push_back(vec3(1, 1, 1));
//	t->push_back(vec3(1, 0, 1));
//	tail->shape->setVertices(t);
//
//	scene->getRoot()->addChild(torso);
//	torso->addChild(head);
//	torso->addChild(rightLeg);
//	torso->addChild(leftLeg);
//	torso->addChild(leftArm);
//	torso->addChild(rightArm);
//	leftLeg->addChild(leftFoot);
//	rightLeg->addChild(rightFoot);
//	leftArm->addChild(leftHand);
//	rightArm->addChild(rightHand);
//	head->addChild(leftEar);
//	head->addChild(rightEar);
//	torso->addChild(tail);
//
//
//	scene->traverse(scene->getRoot(), mat3::identity());
//
//}
//////////////////////////////////////////////////////////////////////////
