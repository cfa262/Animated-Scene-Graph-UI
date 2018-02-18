// window.h
// The Window class has the main Qt components and links together the GUI with your scene graph

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QtWidgets>
#include <QSlider>
#include <QColorDialog>
#include <QColor>
#include <list>
//#include "SceneGraph.h"


//class Data;  // forward declaration of my simple little data class
class Node;   //foward declaration of Node class
class SceneGraph;

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
	~Window();
	Node* getRoot();

protected:
    void keyPressEvent(QKeyEvent *event);
	void save_Frame();
	SceneGraph * getCurrentSceneGraph();
	void setupTreeView(Node * n, QStandardItem * row);
	Node * createAnimal();

	signals:
	
	public slots :
		void on_treeView_currentItemChanged(const QItemSelection &selected, const QItemSelection &deselected);  // Called when a different item in the tree view is selected. 
		void on_xTransSlider_valueChanged(int value); // Called when the slider is slid.
		void transform(Node * current);
		void on_yTransSlider_valueChanged(int value);
		void on_xScaleSlider_valueChanged(int value);
		void on_yScaleSlider_valueChanged(int value);
		void on_rotateDialer_valueChanged(int value);
		void on_animation_valueChanged(int value);
		void on_number_frames();
		void save_Animation();
		void play_Animation();
		void animate();
		void go_Backward();
		void go_Foward();
		void color_Box();
		void add_Node();
		void remove_Node();
		void restartAnimal();

private:
    Ui::Window *ui;				// A Qt internal mechanism
	QStandardItemModel* model;  // Links Qt with your scene graph 
	QStandardItem* tempItem;
	QTimer* timer;

	std::vector<SceneGraph*>* sceneList; //used to store frames and used for animation
	int count;
	SceneGraph* currentScene;

	Node* currentNode;  //pointer to the currently selected "node"
	Node* root;	//variable to store the root
	
};

#endif // WINDOW_H
