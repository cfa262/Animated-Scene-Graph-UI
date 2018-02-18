// Simple Qt application with an OpenGL window. 
// Demonstrates how to create a Qt window with
// an OpenGL window inside it and display
// OpenGL primitives. 
// This is a good website for getting started: http://doc.qt.io/qtvstools/qtvstools-getting-started.html
// The OpenGL code is based on this example: http://www.bogotobogo.com/Qt/Qt5_OpenGL_QGLWidget.php


#include <QApplication>			// These headers are needed for our Qt application
#include <QDesktopWidget>


#include "window.h"				// Declares a window class for the Qt window and interaction with it. 

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);	// High level control for our Qt application. Includes start of an infinite loop
    Window window;					// Create a Qt window. Defined in window.h

	// Set the window size and area. Don't worry about these right now. 
	window.resize(window.sizeHint());	
    int desktopArea = QApplication::desktop()->width() *
                     QApplication::desktop()->height();
    int widgetArea = window.width() * window.height();

	window.setWindowTitle("My Scene Graph Animal");		// Give the window an appropriate title. 

	// A window will not appear until a show method is called. This allows us to setup everything before
	// a user sees it. 
    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        window.show();
    else
        window.showMaximized();

	// window.hide(); // Note there is also a hide method, so you could create a bunch of windows at the start of an app and just show and hide them in the correct contexts.

	return app.exec(); // Starts the infinite loop necessary for event based programming. What happens if this isn't called? 
}
