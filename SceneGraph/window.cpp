//Demonstrating connecting the Qt Tree View widget to another class (like a scene graph)

#include <iostream>
#include <sstream>
#include <string>
#include "window.h"
#include "ui_window.h"
#include "node.h"
#include "myglwidget.h"
#include "SceneGraph.h"

// The code is all put here for ease of explanation. Your node classes would be in a separate .cpp file 
// Some of the other code may be not be in the window constructor. Do a better code design.

Q_DECLARE_METATYPE(Node*);  // Tell Qt that Data is a type.
Q_DECLARE_METATYPE(SceneGraph*);

std::string getNewName() // return a unique name 
{
	static int count = 0;	// keep counting the number of objects

	std::string s;
	std::stringstream out;	// a stream for outputing to a string
	out << count++;			// make the current count into a string
	s = out.str();

	return "Node_" + s;	// append the current count onto the string
}

Window::Window(QWidget *parent) :  // Window constructor
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);				// Standard Qt stuff
	model = new QStandardItemModel();  // Tree views have to be populated with models, so create one
	sceneList = new std::vector<SceneGraph*>();

	// You should be able to traverse your scene graph and set these up as you traverse or you can do it as you create your scene graph. 
	// Also, realize you will need to create and delete these items when you create and delete nodes in your scene graph
	//SceneGraph* scene = new SceneGraph();

	Node* d = new Node("Root");		// Create a new node
	QStandardItem* rootItem = new QStandardItem(QString::fromStdString(d->name));	// These would correspond to your scene graph nodes
	rootItem->setData(QVariant::fromValue(d)); // Store a pointer to data in the tree item so that you can retrieve it when an item is selected. 
	model->appendRow(rootItem);

	Node* animalRoot = createAnimal();
	setupTreeView(animalRoot, rootItem);  // sets up treeview using QStandardItem
	ui->myGLWidget->glScene->root = animalRoot; //sets MyGlWidget root node to torso node
	currentScene = ui->myGLWidget->glScene;

	ui->treeView->setModel(model); // ui needs to include treeView, so create one in Qt Creator and make sure it is named treeView
								   // in the ui, I also made the header of the treeView not visible. 
	ui->treeView->expandAll();	   // show all of the items at all levels 

	// When a user selects an item in the treeView have a signal sent to a method in this class, on_treeView_currentItemChanged
	connect(ui->treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &Window::on_treeView_currentItemChanged);

	// Set up initial sliders values
	ui->xTransSlider->setVisible(FALSE);
	ui->yTransSlider->setVisible(FALSE);
	ui->xTransSlider->setMinimum(-10); ui->xTransSlider->setMaximum(10); ui->xTransSlider->setSingleStep(1); // These are just sample to show how to do it. You decide on appropriate values. To use floats instead of ints, divide the values the sliders return by 100.0
	ui->yTransSlider->setMinimum(-10); ui->yTransSlider->setMaximum(10); ui->yTransSlider->setSingleStep(1);
	//scale slider values
	ui->xScaleSlider->setVisible(FALSE);
	ui->yScaleSlider->setVisible(FALSE);
	ui->xScaleSlider->setMinimum(-10); ui->xScaleSlider->setMaximum(10); ui->xScaleSlider->setSingleStep(1); // These are just sample to show how to do it. You decide on appropriate values. To use floats instead of ints, divide the values the sliders return by 100.0
	ui->yScaleSlider->setMinimum(-10); ui->yScaleSlider->setMaximum(10); ui->yScaleSlider->setSingleStep(1);
	//rotation dial values
	ui->rotationDial->setVisible(FALSE);
	ui->rotationDial->setMinimum(-180); ui->rotationDial->setMaximum(180); ui->rotationDial->setSingleStep(1);
	//animation scroll bar values
	ui->animationScrollBar->setVisible(FALSE);
	ui->animationScrollBar->setMinimum(0); ui->animationScrollBar->setMaximum(0); ui->animationScrollBar->setSingleStep(1);

	// When a user moves a slider/dialer, update the label value to show it
	connect(ui->xTransSlider, &QSlider::valueChanged, this, &Window::on_xTransSlider_valueChanged);
	connect(ui->yTransSlider, &QSlider::valueChanged, this, &Window::on_yTransSlider_valueChanged);
	connect(ui->xScaleSlider, &QSlider::valueChanged, this, &Window::on_xScaleSlider_valueChanged);
	connect(ui->yScaleSlider, &QSlider::valueChanged, this, &Window::on_yScaleSlider_valueChanged);
	connect(ui->rotationDial, &QDial::valueChanged, this, &Window::on_rotateDialer_valueChanged);
	// in animation mode
	connect(ui->animationScrollBar, &QScrollBar::valueChanged, this, &Window::on_animation_valueChanged);

	//when user presses a button in modeling mode
	connect(ui->addButton, &QPushButton::clicked, this, &Window::add_Node);
	connect(ui->removeButton, &QPushButton::clicked, this, &Window::remove_Node);
	connect(ui->magicButton, &QPushButton::clicked, this, &Window::restartAnimal);
	connect(ui->colorPicker, &QPushButton::clicked, this, &Window::color_Box);
	connect(ui->saveFrameButton, &QPushButton::clicked, this, &Window::save_Frame);
	connect(ui->numberFramesButton, &QPushButton::clicked, this, &Window::on_number_frames);

	//when user presses a button in animation mode
	connect(ui->playButton, &QPushButton::clicked, this, &Window::play_Animation);
	connect(ui->backwardButton, &QPushButton::clicked, this, &Window::go_Backward);
	connect(ui->fowardButton, &QPushButton::clicked, this, &Window::go_Foward);
	connect(ui->animateButton, &QPushButton::clicked, this, &Window::save_Animation);


	//timer for playing animation
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
	
	currentNode = NULL; // Currently no node in the treeView is selected, so reflect that. 
}

Window::~Window()
{
    delete ui;
}

Node* Window::getRoot()
{
	return this->root;
}

SceneGraph* Window::getCurrentSceneGraph() {
	return this->currentScene;
}

void Window::setupTreeView(Node* n, QStandardItem* row) {
	QStandardItem* qItem = new QStandardItem(QString::fromStdString(n->name));
	qItem->setData(QVariant::fromValue(n));
	row->appendRow(qItem);
	for (std::list<Node*>::const_iterator it = n->getChildren()->cbegin(); it != n->getChildren()->end(); ++it) {
		setupTreeView((*it), qItem);
	}
}

void Window::keyPressEvent(QKeyEvent *e)
// If the user hits esc, close the program. 
//add other keyPressEvents
{
	if (e->key() == Qt::Key_Escape){
		close();
	}
    else
        QWidget::keyPressEvent(e);
}

// This methods automatically gets called when a different item is selected in the treeView.
// It is in a slot of this class and connected to a signal in the constructor of this class. 
void Window::on_treeView_currentItemChanged(const QItemSelection &selected, const QItemSelection &deselected)  
{
	Node* d;

	QModelIndex index;
	QModelIndexList items = selected.indexes(); // It is possible to select more than one item. 

	foreach(index, items) {				// For each item selected look at the data. 
		d = index.data(Qt::UserRole + 1).value<Node*>();
		//std::cout << "My data is " << d->x << ", " << d->y << std::endl; // Does not actually do anything because there is no console.
	}

	currentNode = d; // keep a pointer to the node that corresponds to the currently selected tree item. Makes things easier. 
	

	ui->xTransSlider->setValue(d->transX);     // Set the values of the slides to what is currently in the node
	ui->yTransSlider->setValue(d->transY);
	ui->xTransSlideLabel->setNum(d->transX);		// Show the value in the label for the x slider
	ui->yTransSlideLabel->setNum(d->transY);     // Show the value in the label for the y slider

	ui->xScaleSlider->setValue(d->scaleX);     // Set the values of the slides to what is currently in the node
	ui->yScaleSlider->setValue(d->scaleY);
	ui->xScaleSliderLabel->setNum(d->scaleX);		// Show the value in the label for the x slider
	ui->yScaleSliderLabel->setNum(d->scaleY);

	ui->rotationDial->setValue(d->rot);
	ui->rotationDialLabel->setNum(d->rot);

	//animation widget
	ui->animationScrollBar->setValue(0);
	ui->animationLabel->setNum(0);

	ui->xTransSlider->setVisible(TRUE);   // Make the sliders visible
	ui->yTransSlider->setVisible(TRUE);
	ui->xScaleSlider->setVisible(TRUE);
	ui->yScaleSlider->setVisible(TRUE);
	ui->rotationDial->setVisible(TRUE);
	ui->animationScrollBar->setVisible(TRUE);

}

// Called when the slider is slid. 
void Window::on_xTransSlider_valueChanged(int value)
{
	float v = value / 1.0;
	ui->xTransSlideLabel->setNum(value);	// Set the label value

	// Update the node value
	if (currentNode != NULL) {
		currentNode->transX = v;
		transform(currentNode);
		//currentNode->translate();
	}
	
	// Update the OpenGL window
	ui->myGLWidget->update();  // Note in this sample code it will not do anything. In yours it should 
}

void Window::on_yTransSlider_valueChanged(int value)
{
	ui->yTransSlideLabel->setNum(value);	// Set the label value
											// Update the node value
	if (currentNode != NULL){
		currentNode->transY = value;
		transform(currentNode);
	}
	// Update the OpenGL window
	ui->myGLWidget->update();  
}

void Window::on_xScaleSlider_valueChanged(int value)
{
	ui->xScaleSliderLabel->setNum(value);	// Set the label value

											// Update the node value
	if (currentNode != NULL) {
		currentNode->scaleX = value;
		transform(currentNode);
	}
	// Update the OpenGL window
	ui->myGLWidget->update();  // Note in this sample code it will not do anything. In yours it should 

}

void Window::on_yScaleSlider_valueChanged(int value)
{
	ui->yScaleSliderLabel->setNum(value);	// Set the label value

											// Update the node value
	if (currentNode != NULL){
		currentNode->scaleY = value;
		transform(currentNode);
	}
		

	// Update the OpenGL window
	ui->myGLWidget->update();  // Note in this sample code it will not do anything. In yours it should 

}

void Window::on_rotateDialer_valueChanged(int value)
{
	float rotValue = value / 1.0;
	ui->rotationDialLabel->setNum(rotValue);	// Set the label value

											// Update the node value
	if (currentNode != NULL) {
		currentNode->rot = rotValue;
		transform(currentNode);
	}
	// Update the OpenGL window
	ui->myGLWidget->update();  // Note in this sample code it will not do anything. In yours it should 

}

void Window::on_number_frames() {
	//enters the number of frames in the timeline
	int value = ui->spinBox->value();
	ui->animationScrollBar->setMinimum(0); ui->animationScrollBar->setMaximum(value); ui->animationScrollBar->setSingleStep(1);
	ui->animationScrollBar->setVisible(TRUE);
	//this->sceneList->resize(value+1);
}

void Window::on_animation_valueChanged(int value) {
	ui->animationLabel->setNum(value);
	count = value;
	std::vector<SceneGraph*>::iterator sit = this->sceneList->begin();
	ui->myGLWidget->glScene = (sit[count]);
	ui->myGLWidget->update();  // should update myGLWidget to show the current frame

}

void Window::save_Animation() {
	//saves the screen output for the series of key frames in animation mode
	SceneGraph* newScene = new SceneGraph();
	newScene->setRoot(this->currentScene->getRoot());
	this->sceneList->push_back(newScene);
	QImage image = ui->myGLWidget->grabFrameBuffer();
	image.save("frame.jpg");
	restartAnimal();
}

void Window::save_Frame() { 
	//saves current frame in modeling mode
	//SceneGraph* nextScene = new SceneGraph(*currentScene);

	SceneGraph* newScene = new SceneGraph();
	newScene->root = currentScene->getRoot();
	this->sceneList->push_back(newScene);

	restartAnimal();  
}

void Window::play_Animation() {
	//starts the timer for the animation
	this->count = 0;
	this->timer->start(500);
}

void Window::animate() {

	//SceneGraph* sg = new SceneGraph(*currentScene);

	if (sceneList->size() != 0 && sceneList != NULL) {
		std::vector<SceneGraph*>::iterator sit = this->sceneList->begin();
		ui->myGLWidget->glScene = (sit[count]);
		count++;
		ui->animationScrollBar->setValue(count - 1);
		ui->animationLabel->setNum(count - 1);
		ui->myGLWidget->update();

		
	}

	if (count >= sceneList->size() && sceneList != NULL) {
		this->timer->stop();
	}
}
void Window::go_Foward() {
	//will go forward one scenegraph
	this->timer->stop();
	if (count != sceneList->size() && sceneList != NULL) {
		std::vector<SceneGraph*>::iterator sit = this->sceneList->begin();
		count++;
		ui->myGLWidget->glScene = (sit[count]);
		ui->myGLWidget->update();
		ui->animationLabel->setNum(count);
		ui->animationScrollBar->setValue(count);
	}
}

void Window::go_Backward() {
	//will go backward one scenegraph
	this->timer->stop();
	if (count != 0 && sceneList != NULL) {
		std::vector<SceneGraph*>::iterator sit = this->sceneList->begin();
		count--;
		ui->myGLWidget->glScene = (sit[count]);
		ui->myGLWidget->update();
		ui->animationLabel->setNum(count);
		ui->animationScrollBar->setValue(count);
	}
}

void Window::color_Box() { 
	QColor color = QColorDialog::getColor(Qt::white, this, "Choose Color");
	if ((color).isValid()) {
		
		currentNode->shape->setColor((float)color.red()/255, (float)color.green()/255, (float)color.blue()/255); 
	}
	ui->myGLWidget->update();
}

void Window::transform(Node* current) {
	/* Create a scale matrix from the x and y sliders' values */
	mat3 scale = mat3::scale2D(current->scaleX, current->scaleY);
	mat3 rotate = mat3::rotation2D(current->rot);
	mat3 translate = mat3::translation2D(current->transX, current->transY);
	current->setTransformation(scale, rotate, translate);
}

void Window::add_Node() {

	Node* n = new Node(getNewName());
	n->setPolyTrue();
	n->shape->setColor(.5, .5, 1);
	n->setTransformation(mat3::identity(), mat3::identity(), mat3::identity());
	std::list <vec3> *v = new std::list<vec3>();
	v->push_back(vec3(-1, -1, 1));
	v->push_back(vec3(-1, 1, 1));
	v->push_back(vec3(1, 1, 1));
	v->push_back(vec3(1, -1, 1));
	n->shape->setVertices(v);

	QModelIndex id = ui->treeView->currentIndex();

	QStandardItem* item1 = new QStandardItem(QString::fromStdString(n->name));	// These would correspond to your scene graph nodes
	item1->setData(QVariant::fromValue(n)); // Store a pointer to data in the tree item so that you can retrieve it when an item is selected. 
	model->appendRow(item1);
	//setupTreeView(currentNode, item);
	
	currentNode->addChild(n);
	//ui->treeView->model()->insertRow(id.row(), id.parent());

	ui->treeView->update();
	ui->myGLWidget->update();
}

void Window::remove_Node() {
	
	QModelIndex id = ui->treeView->currentIndex();
	currentNode->parent->removeNode(currentNode);
	ui->treeView->model()->removeRow(id.row(), id.parent());
	//currentNode->removeNode(currentNode);
	
	ui->treeView->update();
	ui->myGLWidget->update();
}

Node* Window::createAnimal() {

	//model->clear();

	Node* torso = new Node("Torso");
	torso->setPolyTrue();
	torso->shape->setColor(.5, 1, .5);
	torso->transX = 0;
	torso->transY = 0;
	torso->setTransformation(mat3::identity(), mat3::identity(), mat3::identity());
	std::list <vec3> *v = new std::list<vec3>();
	v->push_back(vec3(-4, -2, 1));
	v->push_back(vec3(-4, 2, 1));
	v->push_back(vec3(4, 2, 1));
	v->push_back(vec3(4, -2, 1));
	torso->shape->setVertices(v);

	Node* head = new Node("Head");
	head->setPolyTrue();
	head->shape->setColor(.4, 1, .8);
	head->transX = 6;
	head->transY = 3;
	head->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(head->transX, head->transY));
	std::list <vec3> *h = new std::list<vec3>();
	h->push_back(vec3(-2, -1, 1));
	h->push_back(vec3(-2, 2, 1));
	h->push_back(vec3(0, 3, 1));
	h->push_back(vec3(2, 2, 1));
	h->push_back(vec3(2, -1, 1));
	head->shape->setVertices(h);

	Node* leftEar = new Node("LeftEar");
	leftEar->setPolyTrue();
	leftEar->shape->setColor(.4, .4, .6);
	leftEar->transX = -1;
	leftEar->transY = 2;
	leftEar->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(leftEar->transX, leftEar->transY));
	std::list <vec3> *e = new std::list<vec3>();
	e->push_back(vec3(-1, 0, 1));
	e->push_back(vec3(.5, .5, 1));
	e->push_back(vec3(-.5, 2, 1));
	leftEar->shape->setVertices(e);

	Node* rightEar = new Node("RightEar");
	rightEar->setPolyTrue();
	rightEar->shape->setColor(.4, .4, .6);
	rightEar->transX = 1;
	rightEar->transY = 2;
	rightEar->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(rightEar->transX, rightEar->transY));
	std::list <vec3> *r = new std::list<vec3>();
	r->push_back(vec3(1, 0, 1));
	r->push_back(vec3(-.5, .5, 1));
	r->push_back(vec3(.5, 2, 1));
	rightEar->shape->setVertices(r);

	Node* leftLeg = new Node("LeftLeg");
	leftLeg->setPolyTrue();
	leftLeg->shape->setColor(.25, .25, 1);
	leftLeg->transX = -3;
	leftLeg->transY = -4;
	leftLeg->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(leftLeg->transX, leftLeg->transY));
	std::list <vec3> *lLeg = new std::list<vec3>();
	lLeg->push_back(vec3(-.5, -2, 1));
	lLeg->push_back(vec3(-.5, 2, 1));
	lLeg->push_back(vec3(.5, 2, 1));
	lLeg->push_back(vec3(.5, -2, 1));
	leftLeg->shape->setVertices(lLeg);

	Node* rightLeg = new Node("RightLeg");
	rightLeg->setPolyTrue();
	rightLeg->shape->setColor(.25, .25, 1);
	rightLeg->transX = -1;
	rightLeg->transY = -4;
	rightLeg->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(rightLeg->transX, rightLeg->transY));
	std::list <vec3> *rLeg = new std::list<vec3>();
	rLeg->push_back(vec3(-.5, -2, 1));
	rLeg->push_back(vec3(-.5, 2, 1));
	rLeg->push_back(vec3(.5, 2, 1));
	rLeg->push_back(vec3(.5, -2, 1));
	rightLeg->shape->setVertices(rLeg);

	Node* leftArm = new Node("LeftArm");
	leftArm->setPolyTrue();
	leftArm->shape->setColor(.25, .25, 1);
	leftArm->transX = 1;
	leftArm->transY = -4;
	leftArm->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(leftArm->transX, leftArm->transY));
	std::list <vec3> *lArm = new std::list<vec3>();
	lArm->push_back(vec3(-.5, -2, 1));
	lArm->push_back(vec3(-.5, 2, 1));
	lArm->push_back(vec3(.5, 2, 1));
	lArm->push_back(vec3(.5, -2, 1));
	leftArm->shape->setVertices(lArm);

	Node* rightArm = new Node("RightArm");
	rightArm->setPolyTrue();
	rightArm->shape->setColor(.25, .25, 1);
	rightArm->transX = 3;
	rightArm->transY = -4;
	rightArm->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(rightArm->transX, rightArm->transY));
	std::list <vec3> *rArm = new std::list<vec3>();
	rArm->push_back(vec3(-.5, -2, 1));
	rArm->push_back(vec3(-.5, 2, 1));
	rArm->push_back(vec3(.5, 2, 1));
	rArm->push_back(vec3(.5, -2, 1));
	rightArm->shape->setVertices(rArm);

	Node* leftFoot = new Node("LeftFoot");
	leftFoot->setPolyTrue();
	leftFoot->shape->setColor(1, .5, .5);
	leftFoot->transX = 0;
	leftFoot->transY = -3;
	leftFoot->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(leftFoot->transX, leftFoot->transY));
	std::list <vec3> *lFoot = new std::list<vec3>();
	lFoot->push_back(vec3(-.5, 0, 1));
	lFoot->push_back(vec3(-.5, 1, 1));
	lFoot->push_back(vec3(1.5, 1, 1));
	lFoot->push_back(vec3(1.5, 0, 1));
	leftFoot->shape->setVertices(lFoot);

	Node* rightFoot = new Node("RightFoot");
	rightFoot->setPolyTrue();
	rightFoot->shape->setColor(.75, .25, 1);
	rightFoot->transX = 0;
	rightFoot->transY = -3;
	rightFoot->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(rightFoot->transX, rightFoot->transY));
	std::list <vec3> *rFoot = new std::list<vec3>();
	rFoot->push_back(vec3(-.5, 0, 1));
	rFoot->push_back(vec3(-.5, 1, 1));
	rFoot->push_back(vec3(1.5, 1, 1));
	rFoot->push_back(vec3(1.5, 0, 1));
	rightFoot->shape->setVertices(rFoot);

	Node* leftHand = new Node("LeftHand");
	leftHand->setPolyTrue();
	leftHand->shape->setColor(1, 1, .25);
	leftHand->transX = 0;
	leftHand->transY = -3;
	leftHand->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(leftHand->transX, leftHand->transY));
	std::list <vec3> *lHand = new std::list<vec3>();
	lHand->push_back(vec3(-.5, 0, 1));
	lHand->push_back(vec3(-.5, 1, 1));
	lHand->push_back(vec3(1.5, 1, 1));
	lHand->push_back(vec3(1.5, 0, 1));
	leftHand->shape->setVertices(lHand);

	Node* rightHand = new Node("RightHand");
	rightHand->setPolyTrue();
	rightHand->shape->setColor(1, .5, 0);
	rightHand->transX = 0;
	rightHand->transY = -3;
	rightHand->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(rightHand->transX, rightHand->transY));
	std::list <vec3> *rHand = new std::list<vec3>();
	rHand->push_back(vec3(-.5, 0, 1));
	rHand->push_back(vec3(-.5, 1, 1));
	rHand->push_back(vec3(1.5, 1, 1));
	rHand->push_back(vec3(1.5, 0, 1));
	rightHand->shape->setVertices(rHand);

	Node* tail = new Node("Tail");
	tail->setPolyTrue();
	tail->shape->setColor(.5, .5, .5);
	tail->transX = -5;
	tail->transY = 1;
	tail->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(tail->transX, tail->transY));
	std::list <vec3> *t = new std::list<vec3>();
	t->push_back(vec3(-2, 0, 1));
	t->push_back(vec3(-2, 1, 1));
	t->push_back(vec3(-1, 4, 1));
	t->push_back(vec3(-1, 1, 1));
	t->push_back(vec3(1, 1, 1));
	t->push_back(vec3(1, 0, 1));
	tail->shape->setVertices(t);

	torso->addChild(head);
	torso->addChild(rightLeg);
	torso->addChild(leftLeg);
	torso->addChild(leftArm);
	torso->addChild(rightArm);
	leftLeg->addChild(leftFoot);
	rightLeg->addChild(rightFoot);
	leftArm->addChild(leftHand);
	rightArm->addChild(rightHand);
	head->addChild(leftEar);
	head->addChild(rightEar);
	torso->addChild(tail);

	return torso;
}

void Window::restartAnimal() {

	model->clear();

	SceneGraph* newScene = new SceneGraph();  //creates a new Scene Graph

	Node* d = new Node("Root");		// Create a new node
	QStandardItem* rootItem = new QStandardItem(QString::fromStdString(d->name));	// These would correspond to your scene graph nodes
	rootItem->setData(QVariant::fromValue(d)); // Store a pointer to data in the tree item so that you can retrieve it when an item is selected. 
	model->appendRow(rootItem);

	Node* torso = new Node("Torso");
	//QStandardItem* item1 = new QStandardItem(QString::fromStdString(torso->name));
	//item1->setData(QVariant::fromValue(torso)); // Store a pointer to data in the tree item. 
	//item0->appendRow(item1);

	torso->setPolyTrue();
	torso->shape->setColor(.5, 1, .5);
	torso->transX = 0;
	torso->transY = 0;
	torso->setTransformation(mat3::identity(), mat3::identity(), mat3::identity());
	std::list <vec3> *v = new std::list<vec3>();
	v->push_back(vec3(-4, -2, 1));
	v->push_back(vec3(-4, 2, 1));
	v->push_back(vec3(4, 2, 1));
	v->push_back(vec3(4, -2, 1));
	torso->shape->setVertices(v);

	Node* head = new Node("Head");
	head->setPolyTrue();
	head->shape->setColor(.4, 1, .8);
	head->transX = 6;
	head->transY = 3;
	head->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(head->transX, head->transY));
	std::list <vec3> *h = new std::list<vec3>();
	h->push_back(vec3(-2, -1, 1));
	h->push_back(vec3(-2, 2, 1));
	h->push_back(vec3(0, 3, 1));
	h->push_back(vec3(2, 2, 1));
	h->push_back(vec3(2, -1, 1));
	head->shape->setVertices(h);

	Node* leftEar = new Node("LeftEar");
	leftEar->setPolyTrue();
	leftEar->shape->setColor(.4, .4, .6);
	leftEar->transX = -1;
	leftEar->transY = 2;
	leftEar->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(leftEar->transX, leftEar->transY));
	std::list <vec3> *e = new std::list<vec3>();
	e->push_back(vec3(-1, 0, 1));
	e->push_back(vec3(.5, .5, 1));
	e->push_back(vec3(-.5, 2, 1));
	leftEar->shape->setVertices(e);

	Node* rightEar = new Node("RightEar");
	rightEar->setPolyTrue();
	rightEar->shape->setColor(.4, .4, .6);
	rightEar->transX = 1;
	rightEar->transY = 2;
	rightEar->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(rightEar->transX, rightEar->transY));
	std::list <vec3> *r = new std::list<vec3>();
	r->push_back(vec3(1, 0, 1));
	r->push_back(vec3(-.5, .5, 1));
	r->push_back(vec3(.5, 2, 1));
	rightEar->shape->setVertices(r);

	Node* leftLeg = new Node("LeftLeg");
	leftLeg->setPolyTrue();
	leftLeg->shape->setColor(.25, .25, 1);
	leftLeg->transX = -3;
	leftLeg->transY = -4;
	leftLeg->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(leftLeg->transX, leftLeg->transY));
	std::list <vec3> *lLeg = new std::list<vec3>();
	lLeg->push_back(vec3(-.5, -2, 1));
	lLeg->push_back(vec3(-.5, 2, 1));
	lLeg->push_back(vec3(.5, 2, 1));
	lLeg->push_back(vec3(.5, -2, 1));
	leftLeg->shape->setVertices(lLeg);

	Node* rightLeg = new Node("RightLeg");
	rightLeg->setPolyTrue();
	rightLeg->shape->setColor(.25, .25, 1);
	rightLeg->transX = -1;
	rightLeg->transY = -4;
	rightLeg->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(rightLeg->transX, rightLeg->transY));
	std::list <vec3> *rLeg = new std::list<vec3>();
	rLeg->push_back(vec3(-.5, -2, 1));
	rLeg->push_back(vec3(-.5, 2, 1));
	rLeg->push_back(vec3(.5, 2, 1));
	rLeg->push_back(vec3(.5, -2, 1));
	rightLeg->shape->setVertices(rLeg);

	Node* leftArm = new Node("LeftArm");
	leftArm->setPolyTrue();
	leftArm->shape->setColor(.25, .25, 1);
	leftArm->transX = 1;
	leftArm->transY = -4;
	leftArm->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(leftArm->transX, leftArm->transY));
	std::list <vec3> *lArm = new std::list<vec3>();
	lArm->push_back(vec3(-.5, -2, 1));
	lArm->push_back(vec3(-.5, 2, 1));
	lArm->push_back(vec3(.5, 2, 1));
	lArm->push_back(vec3(.5, -2, 1));
	leftArm->shape->setVertices(lArm);

	Node* rightArm = new Node("RightArm");
	rightArm->setPolyTrue();
	rightArm->shape->setColor(.25, .25, 1);
	rightArm->transX = 3;
	rightArm->transY = -4;
	rightArm->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(rightArm->transX, rightArm->transY));
	std::list <vec3> *rArm = new std::list<vec3>();
	rArm->push_back(vec3(-.5, -2, 1));
	rArm->push_back(vec3(-.5, 2, 1));
	rArm->push_back(vec3(.5, 2, 1));
	rArm->push_back(vec3(.5, -2, 1));
	rightArm->shape->setVertices(rArm);

	Node* leftFoot = new Node("LeftFoot");
	leftFoot->setPolyTrue();
	leftFoot->shape->setColor(1, .5, .5);
	leftFoot->transX = 0;
	leftFoot->transY = -3;
	leftFoot->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(leftFoot->transX, leftFoot->transY));
	std::list <vec3> *lFoot = new std::list<vec3>();
	lFoot->push_back(vec3(-.5, 0, 1));
	lFoot->push_back(vec3(-.5, 1, 1));
	lFoot->push_back(vec3(1.5, 1, 1));
	lFoot->push_back(vec3(1.5, 0, 1));
	leftFoot->shape->setVertices(lFoot);

	Node* rightFoot = new Node("RightFoot");
	rightFoot->setPolyTrue();
	rightFoot->shape->setColor(.75, .25, 1);
	rightFoot->transX = 0;
	rightFoot->transY = -3;
	rightFoot->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(rightFoot->transX, rightFoot->transY));
	std::list <vec3> *rFoot = new std::list<vec3>();
	rFoot->push_back(vec3(-.5, 0, 1));
	rFoot->push_back(vec3(-.5, 1, 1));
	rFoot->push_back(vec3(1.5, 1, 1));
	rFoot->push_back(vec3(1.5, 0, 1));
	rightFoot->shape->setVertices(rFoot);

	Node* leftHand = new Node("LeftHand");
	leftHand->setPolyTrue();
	leftHand->shape->setColor(1, 1, .25);
	leftHand->transX = 0;
	leftHand->transY = -3;
	leftHand->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(leftHand->transX, leftHand->transY));
	std::list <vec3> *lHand = new std::list<vec3>();
	lHand->push_back(vec3(-.5, 0, 1));
	lHand->push_back(vec3(-.5, 1, 1));
	lHand->push_back(vec3(1.5, 1, 1));
	lHand->push_back(vec3(1.5, 0, 1));
	leftHand->shape->setVertices(lHand);

	Node* rightHand = new Node("RightHand");
	rightHand->setPolyTrue();
	rightHand->shape->setColor(1, .5, 0);
	rightHand->transX = 0;
	rightHand->transY = -3;
	rightHand->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(rightHand->transX, rightHand->transY));
	std::list <vec3> *rHand = new std::list<vec3>();
	rHand->push_back(vec3(-.5, 0, 1));
	rHand->push_back(vec3(-.5, 1, 1));
	rHand->push_back(vec3(1.5, 1, 1));
	rHand->push_back(vec3(1.5, 0, 1));
	rightHand->shape->setVertices(rHand);

	Node* tail = new Node("Tail");
	tail->setPolyTrue();
	tail->shape->setColor(.5, .5, .5);
	tail->transX = -5;
	tail->transY = 1;
	tail->setTransformation(mat3::identity(), mat3::identity(), mat3::translation2D(tail->transX, tail->transY));
	std::list <vec3> *t = new std::list<vec3>();
	t->push_back(vec3(-2, 0, 1));
	t->push_back(vec3(-2, 1, 1));
	t->push_back(vec3(-1, 4, 1));
	t->push_back(vec3(-1, 1, 1));
	t->push_back(vec3(1, 1, 1));
	t->push_back(vec3(1, 0, 1));
	tail->shape->setVertices(t);


	//ui->myGLWidget->glScene->getRoot()->addChild(torso);
	//currentScene->root->addChild(torso);
	newScene->root->addChild(torso);
	torso->addChild(head);
	torso->addChild(rightLeg);
	torso->addChild(leftLeg);
	torso->addChild(leftArm);
	torso->addChild(rightArm);
	leftLeg->addChild(leftFoot);
	rightLeg->addChild(rightFoot);
	leftArm->addChild(leftHand);
	rightArm->addChild(rightHand);
	head->addChild(leftEar);
	head->addChild(rightEar);
	torso->addChild(tail);

	ui->myGLWidget->glScene->root = newScene->root;
	setupTreeView(torso, rootItem);
	currentScene = ui->myGLWidget->glScene;

	////d = new Node(2, 3, "Head"); // You can hard code the names or use this function defined above
	//QStandardItem* item2 = new QStandardItem(QString::fromStdString(head->name));
	//item2->setData(QVariant::fromValue(head)); // Store a pointer to data in the tree item. 
	//item1->appendRow(item2);

	////d = new Node(1, 4, "Left Ear"); // You can hard code the names or use this function defined above
	//QStandardItem* item8 = new QStandardItem(QString::fromStdString(leftEar->name));
	//item8->setData(QVariant::fromValue(leftEar)); // Store a pointer to data in the tree item. 
	//item2->appendRow(item8);

	////d = new Node(5, 6, "Right Ear"); // You can hard code the names or use this function defined above
	//QStandardItem* item9 = new QStandardItem(QString::fromStdString(rightEar->name));
	//item9->setData(QVariant::fromValue(rightEar)); // Store a pointer to data in the tree item. 
	//item2->appendRow(item9);

	////d = new Node(4, 5, "Right Arm");
	//QStandardItem* item3 = new QStandardItem(QString::fromStdString(rightArm->name));
	//item3->setData(QVariant::fromValue(rightArm)); // Store a pointer to data in the tree item. 
	//item1->appendRow(item3);

	////d = new Node(4, 5, "Right Hand");
	//QStandardItem* item10 = new QStandardItem(QString::fromStdString(rightHand->name));
	//item10->setData(QVariant::fromValue(rightHand)); // Store a pointer to data in the tree item. 
	//item3->appendRow(item10);

	////d = new Node(5, 6,"Left Arm");
	//QStandardItem* item4 = new QStandardItem(QString::fromStdString(leftArm->name));
	//item4->setData(QVariant::fromValue(leftArm)); // Store a pointer to data in the tree item. 
	//item1->appendRow(item4);

	////d = new Node(4, 5, "Left Hand");
	//QStandardItem* item11 = new QStandardItem(QString::fromStdString(leftHand->name));
	//item11->setData(QVariant::fromValue(leftHand)); // Store a pointer to data in the tree item. 
	//item4->appendRow(item11);

	////d = new Node(5, 6, "Right Leg");
	//QStandardItem* item5 = new QStandardItem(QString::fromStdString(rightLeg->name));
	//item5->setData(QVariant::fromValue(rightLeg)); // Store a pointer to data in the tree item. 
	//item1->appendRow(item5);

	////d = new Node(4, 5, "Right Foot");
	//QStandardItem* item12 = new QStandardItem(QString::fromStdString(rightFoot->name));
	//item12->setData(QVariant::fromValue(rightFoot)); // Store a pointer to data in the tree item. 
	//item5->appendRow(item12);

	////d = new Node(5, 6, "Left Leg");
	//QStandardItem* item6 = new QStandardItem(QString::fromStdString(leftLeg->name));
	//item6->setData(QVariant::fromValue(leftLeg)); // Store a pointer to data in the tree item. 
	//item1->appendRow(item6);

	////d = new Node(4, 5, "Left Foot");
	//QStandardItem* item13 = new QStandardItem(QString::fromStdString(leftFoot->name));
	//item13->setData(QVariant::fromValue(leftFoot)); // Store a pointer to data in the tree item. 
	//item6->appendRow(item13);

	////d = new Node(2, 3, "Tail"); // You can hard code the names or use this function defined above
	//QStandardItem* item7 = new QStandardItem(QString::fromStdString(tail->name));
	//item7->setData(QVariant::fromValue(tail)); // Store a pointer to data in the tree item. 
	//item1->appendRow(item7);

	//ui->myGLWidget->sceneRoot = scene->getRoot();
	//ui->myGLWidget->glScene->root = scene->getRoot();
	ui->myGLWidget->update();
	ui->treeView->expandAll();
}
