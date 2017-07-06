
// main.cpp

// main source file for client application. 

#include "view.h"

#include <iostream>
#include <QApplication>

using namespace std;

// Main entry point for application. Instantiates a QApplication
// instance and a Window instance. Executes the application.

int main (int argc, char ** argv) 
{
	QApplication app(argc, argv);

	// From "view.h"
	Window window;

	window.show();

	return app.exec();
}
