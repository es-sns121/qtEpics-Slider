#include "view.h"

#include <iostream>
#include <QApplication>

using namespace std;

/* Main entry point for application. Instantiates a QApplication
 * instance and a Window instance. Execute the application.
 * */

int main (int argc, char ** argv) 
{
	QApplication app(argc, argv);

/* From "view.h" */
	Window window;

	window.show();

	return app.exec();
}
