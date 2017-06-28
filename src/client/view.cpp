#include "view.h"

#include <QBoxLayout>

Window::Window(QWidget * Parent)
{
	model = new Model("testRecord");
	
	setMinimumSize(300, 150);

	progress_bar = new QProgressBar(this);
	
	progress_bar->setRange(
		(int) model->getRangeMin(),
		(int) model->getRangeMax());

	progress_bar->setValue(
		model->getValue());

/* Get rid of the % in the progress bar's text */
	progress_bar->setFormat("%p");

	slider = new QSlider(this);
	slider->setOrientation(Qt::Horizontal);
	slider->setRange(
		(int) model->getRangeMin(),
		(int) model->getRangeMax());
	slider->setValue(
		model->getValue());

	QObject::connect(slider, SIGNAL (valueChanged(int)), progress_bar, SLOT (setValue(int)));
	
	QObject::connect(slider, SIGNAL (valueChanged(int)), this , SLOT (updateValue(int)));

	QBoxLayout * mainLayout = new QBoxLayout(QBoxLayout::TopToBottom);

	mainLayout->addWidget(progress_bar);
	mainLayout->addWidget(slider);

	setLayout(mainLayout);

	setWindowTitle(tr("qtEpics slider client"));
}

void Window::updateValue(const int & value) {
	model->putValue(value);
}
