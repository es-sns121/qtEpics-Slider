#include "view.h"

#include <iostream>
#include <QApplication>

using namespace std;

int main (int argc, char ** argv) 
{
	QApplication app(argc, argv);

	Window window;

	window.show();

	return app.exec();
}
