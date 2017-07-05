
// view.cpp

// Source file that implements member functions defined in 'view.h'

#include "view.h"

#include <iostream>
using std::cout;
using std::endl;

#include <QBoxLayout>

Window::Window(QWidget * Parent)
{
	model = new Model((void*) this, "testRecord");

// Connect the view's static callback wrapper function to the model
	model->setCallback(&WrapperToCallback);

	int rangeLow = (int) model->getRangeMin();
	int rangeHigh = (int) model->getRangeMax();
	int value = model->getValue();

	progress_bar = new QProgressBar(this);
	progress_bar->setRange(rangeLow, rangeHigh);
	progress_bar->setValue(value);		
// Get rid of the '%' in the progress bar's text.
	progress_bar->setFormat("%p");

	slider = new QSlider(this);
	slider->setOrientation(Qt::Horizontal);
	slider->setRange(rangeLow, rangeHigh);
	slider->setValue(value);
	slider->setTracking(false);						// To prevent spamming the updateModelValue() call, we do not enable slider tracking
													// This means that only the final position of the slider will emit a 'value changed'
													// signal.

// Connect the slider widgets position to the progress bar widgets value.
	QObject::connect(slider, SIGNAL (sliderMoved(int)), progress_bar, SLOT (setValue(int)));

// Connect the slider widget's 'value changed' to the view's updateModelValue() function call.
	QObject::connect(slider, SIGNAL (valueChanged(int)), this , SLOT (updateModelValue(int)));

// Connect the view's 'value changed' signal to the updateViewValue() function call.
	QObject::connect(this, SIGNAL (valueChanged(int)), this, SLOT (updateViewValue(int)));

	textBox = new QTextEdit(this);
	textBox->setReadOnly(true);
	textBox->setMinimumSize(150, 150);

// Formatting main window

	setMinimumSize(450, 200);
	
	QBoxLayout * mainLayout = new QBoxLayout(QBoxLayout::TopToBottom);

	mainLayout->addWidget(progress_bar);
	mainLayout->addWidget(slider);
	mainLayout->addWidget(textBox);

	setLayout(mainLayout);

	setWindowTitle(tr("qtEpics slider client"));
}

// Calls the model's putValue() method. This writes a new value to the record held
// on the server.
void Window::updateModelValue(const int & value) {
	model->putValue(value);
}

// Static wrapper function to window's callback member function
void Window::WrapperToCallback(void * ptrToObj, const int & value)
{
	Window * window = (Window *) ptrToObj;
	window->callback(value);
}

// Updates value of view when model is updated by its monitor
void Window::updateViewValue(const int & value)
{
	// QString text;
	
	// Block signal emition. This prevents signal loops when 'setValue' is called
	blockSignals(true);

	// Update widget values based on the new value from the model
	slider->setValue(value);
	progress_bar->setValue(value);
	// textBox->setText(text);	

	// Unblock signal emition
	blockSignals(false);
}

// Emit valueChanged. This is used instead of calling 'setValue' on the widgets directly
// becuase it would be called from a seperate thread. This allows the main event thread to 
// handle repaints 
void Window::callback(const int & value)
{
	emit valueChanged(value);
}
