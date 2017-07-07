
// view.cpp

// Source file that implements member functions defined in 'view.h'

#include "view.h"

#include <iostream>
#include <sstream>
using std::cout;
using std::endl;
using std::ostringstream;
using std::string;

#include <QBoxLayout>
#include <QGraphicsColorizeEffect>

Limits::Limits(QWidget * parent)
{
	initLowAlarm();
	initLowWarning();
	initHighWarning();
	initHighAlarm();
	
	QBoxLayout * mainLayout = new QBoxLayout(QBoxLayout::LeftToRight);

	mainLayout->addWidget(lowAlarm);
	mainLayout->addWidget(lowWarning);
	mainLayout->addWidget(highWarning);
	mainLayout->addWidget(highAlarm);

	setLayout(mainLayout);
}
void Limits::initLowAlarm()
{
	lowAlarm = new QLabel(this);
	lowAlarm->setFrameStyle(QFrame::VLine || QFrame::Raised);
	
	QGraphicsColorizeEffect * lowAlarmEffect = new QGraphicsColorizeEffect;
	lowAlarmEffect->setStrength(1.0);
	lowAlarmEffect->setColor(Qt::darkRed);
	lowAlarm->setGraphicsEffect(lowAlarmEffect);
}

void Limits::initLowWarning()
{
	lowWarning = new QLabel(this);
	lowWarning->setFrameStyle(QFrame::VLine || QFrame::Raised);
	
	QGraphicsColorizeEffect * lowWarningEffect = new QGraphicsColorizeEffect;
	lowWarningEffect->setStrength(1.0);
	lowWarningEffect->setColor(Qt::darkYellow);
	lowWarning->setGraphicsEffect(lowWarningEffect);
}

void Limits::initHighWarning()
{
	highWarning = new QLabel(this);
	highWarning->setFrameStyle(QFrame::VLine || QFrame::Raised);
	
	QGraphicsColorizeEffect * highWarningEffect = new QGraphicsColorizeEffect;
	highWarningEffect->setStrength(1.0);
	highWarningEffect->setColor(Qt::darkYellow);
	highWarning->setGraphicsEffect(highWarningEffect);
}

void Limits::initHighAlarm()
{
	highAlarm = new QLabel(this);
	highAlarm->setFrameStyle(QFrame::VLine || QFrame::Raised);
	
	QGraphicsColorizeEffect * highAlarmEffect = new QGraphicsColorizeEffect;
	highAlarmEffect->setStrength(1.0);
	highAlarmEffect->setColor(Qt::darkRed);
	highAlarm->setGraphicsEffect(highAlarmEffect);
}

const char * doubleToString(const double & value) 
{
	ostringstream ostr;
	ostr << value;
	string str = ostr.str();
	
	return str.c_str();
}

void Limits::setLowAlarm (const double & value)
{
	string str("low alarm: ");
	str += string(doubleToString(value));
	lowAlarm->setText(QString(str.c_str()));
}

void Limits::setLowWarning (const double & value)
{
	string str("low warning: ");
	str += string(doubleToString(value));
	lowWarning->setText(QString(str.c_str()));
}

void Limits::setHighWarning (const double & value)
{
	string str("high warning: ");
	str += string(doubleToString(value));
	highWarning->setText(QString(str.c_str()));
}

void Limits::setHighAlarm (const double & value)
{
	string str("high alarm: ");
	str += string(doubleToString(value));
	highAlarm->setText(QString(str.c_str()));
}

Window::Window(QWidget * parent)
{
	model = new Model((void*) this, "testRecord");

	// Connect the view's static callback wrapper function to the model
	model->setCallback(&WrapperToCallback);

	int rangeLow = (int) model->getRangeMin();
	int rangeHigh = (int) model->getRangeMax();
	int value = model->getValue();

	initProgressBar(rangeLow, rangeHigh, value);
	
	initSlider(rangeLow, rangeHigh, value);
	
	initCheckbox();

	initLimits();

	initTextbox();
	
	connectWidgets();
	
	formatWindow();
}

void Window::initProgressBar(
	const int & rangeLow,
	const int & rangeHigh,
	const int & value)
{
	progress_bar = new QProgressBar(this);
	progress_bar->setRange(rangeLow, rangeHigh);
	progress_bar->setValue(value);		
	// Get rid of the '%' in the progress bar's text.
	progress_bar->setFormat("%p");

	updateProgressBarColor(value);
}

void Window::initCheckbox()
{
	// The checkbox allows enabling/disabling of the slider's value tracking mode.
	checkBox = new QCheckBox("Slider Value Tracking", this);
	checkBox->setToolTip(tr("Slider value tracking is disabled."));
}

void Window::initSlider(
	const int & rangeLow,
	const int & rangeHigh,
	const int & value)
{
	slider = new QSlider(this);
	slider->setOrientation(Qt::Horizontal);
	slider->setRange(rangeLow, rangeHigh);
	slider->setValue(value);
	slider->setTracking(false);		// To prevent spamming the updateModelValue() call, we disable slider 
									// tracking by default. This means that only the final position of the 
									// slider will emit a 'value changed' signal.
}

void Window::initLimits()
{
	limits = new Limits(this);
	
	limits->setLowAlarm(model->getLowAlarm());
	limits->setLowWarning(model->getLowWarning());
	limits->setHighWarning(model->getHighWarning());
	limits->setHighAlarm(model->getHighAlarm());
}

void Window::initTextbox()
{
	textBox = new QTextEdit(this);
	textBox->setReadOnly(true);
	textBox->setMinimumSize(150, 150);
	textBox->setText(getModelText());
}

void Window::connectWidgets()
{
	// Connect the slider widget's position to the progress bar widget's value.
	QObject::connect(slider, SIGNAL (sliderMoved(int)), progress_bar, SLOT (setValue(int)));

	// Connect the slider widget's 'value changed' to the view's updateModelValue() function call.
	QObject::connect(slider, SIGNAL (valueChanged(int)), this , SLOT (updateModelValue(int)));

	// Connect the view's 'value changed' signal to the updateViewValue() function call.
	QObject::connect(this, SIGNAL (valueChanged(int)), this, SLOT (updateViewValue(int)));

	// Connect the check box's stateChanged signal to the setSliderTracking() function call.
	QObject::connect(checkBox, SIGNAL (stateChanged(int)), this, SLOT(setSliderTracking(int)));
}

void Window::formatWindow()
{
	setMinimumSize(450, 200);
	
	QBoxLayout * mainLayout = new QBoxLayout(QBoxLayout::TopToBottom);

	mainLayout->addWidget(progress_bar);
	mainLayout->addWidget(checkBox);
	mainLayout->addWidget(slider);
	mainLayout->addWidget(limits);
	mainLayout->addWidget(textBox);
	
	setLayout(mainLayout);

	setWindowTitle(tr("qtEpics slider client"));
}

void Window::closeEvent(QCloseEvent * event)
{
	// TODO: How to shut down cleanly?
	//	Need to stop monitor/channel somehow...
	
	event->accept();
}

QString Window::getModelText()
{
	std::string ret = model->getText();
	return QString(ret.c_str());
}

// Calls the model's putValue() method. This writes a new value to the record held
// on the server.
void Window::updateModelValue(const int & value) {
	model->putValue(value);
}

// Updates value of view when model is updated by its monitor
void Window::updateViewValue(const int & value)
{
	QString text = getModelText();

	// Block signal emition. This prevents signal loops when 'setValue' is called
	blockSignals(true);

	// Update widget values based on the new value from the model
	slider->setValue(value);
	progress_bar->setValue(value);
	updateProgressBarColor(value);
	textBox->setText(text);	

	// Unblock signal emition
	blockSignals(false);
}

void Window::updateProgressBarColor(const int & value)
{	
	static double alarmLow    = model->getLowAlarm();
	static double warningLow  = model->getLowWarning();
	static double warningHigh = model->getHighWarning();
	static double alarmHigh   = model->getHighAlarm();

	static QGraphicsColorizeEffect * gEffect = new QGraphicsColorizeEffect;

	if (value <= alarmLow || value >= alarmHigh) {
		// set color to red
		gEffect->setStrength(1.0);
		gEffect->setColor(Qt::darkRed);
		progress_bar->setGraphicsEffect(gEffect);
	
	} else if (value <= warningLow || value >= warningHigh) {
		// set color to yellow
		gEffect->setStrength(1.0);
		gEffect->setColor(Qt::darkYellow);
		progress_bar->setGraphicsEffect(gEffect);
	
	} else {

		gEffect->setStrength(0.0);
		progress_bar->setGraphicsEffect(gEffect);
	
	}
}


// Sets slider value tracking to current checkbox state.
// Unchecked -> disabled
// Checed	 -> enabled
void Window::setSliderTracking(const int & state)
{
	if (state == Qt::Unchecked) {
		
		slider->setTracking(false);		
		checkBox->setToolTip(tr("Slider value tracking is disabled."));
	
	} else if (state == Qt::Checked) {
	
		slider->setTracking(true);
		checkBox->setToolTip(tr("Slider value tracking is enabled."));
	
	} else {
		// error. checkbox tristate is somehow enabled.	
		// TODO: 
		//	what to do? Exception? Exit? 
		//	Currently ignoring error and setting slider into safest tracking
		//	setting .
		slider->setTracking(false);		
	}
}

// Static wrapper function to window's callback member function
void Window::WrapperToCallback(void * ptrToObj, const int & value)
{
	Window * window = (Window *) ptrToObj;
	window->callback(value);
}

// This is used instead of calling 'setValue' on the widgets directly becuase it would be 
// called from a seperate thread. This allows the main event thread to handle repaints 
void Window::callback(const int & value)
{
	emit valueChanged(value);
}
