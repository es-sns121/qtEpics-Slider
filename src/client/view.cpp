#include "view.h"

#include <QBoxLayout>

Window::Window(QWidget * Parent)
{
	model = new Model("testRecord");
	
	setMinimumSize(300, 150);

	int rangeLow = (int) model->getRangeMin();
	int rangeHigh = (int) model->getRangeMax();
	int value = model->getValue();

	progress_bar = new QProgressBar(this);
	progress_bar->setRange(rangeLow, rangeHigh);
	progress_bar->setValue(value);		
/* Get rid of the '%' in the progress bar's text */
	progress_bar->setFormat("%p");

	slider = new QSlider(this);
	slider->setOrientation(Qt::Horizontal);
	slider->setRange(rangeLow, rangeHigh);
	slider->setValue(value);

/* Connect the slider widget to the progress bar widget */
	QObject::connect(slider, SIGNAL (valueChanged(int)), progress_bar, SLOT (setValue(int)));
/* Connect the slider widget to the model's updateValue() function call. */
	QObject::connect(slider, SIGNAL (valueChanged(int)), this , SLOT (updateValue(int)));

/* Formatting main window */

	QBoxLayout * mainLayout = new QBoxLayout(QBoxLayout::TopToBottom);

	mainLayout->addWidget(progress_bar);
	mainLayout->addWidget(slider);

	setLayout(mainLayout);

	setWindowTitle(tr("qtEpics slider client"));
}

/* Calls the model's putValue() method. This writes a new value to the record held
 * on the server.
 * */
void Window::updateValue(const int & value) {
	model->putValue(value);
}
