
// view.cpp

// Source file that implements member functions defined in 'view.h'

#include "view.h"

#include <stdlib.h>
#include <iostream>
#include <sstream>

#include <QBoxLayout>
#include <QGraphicsColorizeEffect>
	
using namespace std;

// MISC ===========================================================================================

const char * longToString(const long & value)
{
	ostringstream ostr;
	ostr << value;
	string str = ostr.str();
	
	return str.c_str();
}

const char * doubleToString(const double & value) 
{
	ostringstream ostr;
	ostr << value;
	string str = ostr.str();
	
	return str.c_str();
}

const char * booleanToString(const bool & value)
{
	if (value)
		return "true";
	else
		return "false";
}

// WIDNOW =========================================================================================

Window::Window(QWidget * parent) {
	model = new Model((void *) this, "testRecord");

	// Connect the view's static callback wrapper function to the model
	model->setCallback(&WrapperToCallback);

	dataTab   = new DataTab(this, model);
	sliderTab = new SliderTab(this, model), 
	structTab = new StructTab(this, model); 	

	tabWidget = new QTabWidget(this);

	tabWidget->addTab(dataTab, "Record Data");
	tabWidget->addTab(sliderTab, "Record Slider");
	tabWidget->addTab(structTab, "Record Structure");

	// Connect the view's 'value changed' signal to the updateViewValue() function call.
	QObject::connect(this, SIGNAL (valueChanged(int)), this, SLOT (updateViewValue(int)));

	setMinimumSize(450, 200);

	QBoxLayout * layout = new QBoxLayout(QBoxLayout::TopToBottom);
	layout->addWidget(tabWidget);
	setLayout(layout);
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

// Updates value of view when model is updated by its monitor
void Window::updateViewValue(const int & value)
{
	// Block signal emition. This prevents signal loops when 'setValue' is called
	blockSignals(true);

	// Update widget values based on the new value from the model

	dataTab->updateData();
	sliderTab->updateData(value);
	structTab->updateData();

	// Unblock signal emition
	blockSignals(false);
}

void Window::closeEvent(QCloseEvent * event)
{
	// TODO: How to shut down cleanly?
	//	Need to stop monitor/channel somehow...
	
	event->accept();
}

// DATA TAB =======================================================================================

DataTab::DataTab(QWidget * parent, Model * model)
{
	this->model = model;
	
	initLabels();

	formatDataTab();

	updateData();
}

void DataTab::initLabels()
{
	_long    = new QLabel(this);
	_double  = new QLabel(this);
	_string  = new QLabel(this);
	_boolean = new QLabel(this);
}

void DataTab::formatDataTab()
{
	QBoxLayout * layout = new QBoxLayout(QBoxLayout::TopToBottom);

	layout->addWidget(_long);
	layout->addWidget(_double);
	layout->addWidget(_string);
	layout->addWidget(_boolean);

	setLayout(layout);
}	

void DataTab::updateData()
{
	long long_value = model->getLong();
	string long_string = "long: " + string(longToString(long_value));
	_long->setText(QString(long_string.c_str()));	
	
	double double_value = model->getDouble();
	string double_string = "double: " + string(doubleToString(double_value));
	_double->setText(QString(double_string.c_str()));
	
	string string_value = model->getString();
	string string_string = "string: " + string_value;
	_string->setText(QString(string_string.c_str()));
	
	bool boolean_value = model->getBoolean();
	string boolean_string = "boolean: " + string(booleanToString(boolean_value));
	_boolean->setText(QString(boolean_string.c_str()));
}

// LIMITS OBJECT ==================================================================================

Limits::Limits(QWidget * parent)
{
	initLowAlarm();
	initLowWarning();
	initHighWarning();
	initHighAlarm();

	setMinimumHeight(30);
	setMaximumHeight(100);

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
	lowAlarm->setWordWrap(true);
}

void Limits::initLowWarning()
{
	lowWarning = new QLabel(this);
	lowWarning->setFrameStyle(QFrame::VLine || QFrame::Raised);
	
	QGraphicsColorizeEffect * lowWarningEffect = new QGraphicsColorizeEffect;
	lowWarningEffect->setStrength(1.0);
	lowWarningEffect->setColor(Qt::darkYellow);
	lowWarning->setGraphicsEffect(lowWarningEffect);
	lowWarning->setWordWrap(true);
}

void Limits::initHighWarning()
{
	highWarning = new QLabel(this);
	highWarning->setFrameStyle(QFrame::VLine || QFrame::Raised);
	
	QGraphicsColorizeEffect * highWarningEffect = new QGraphicsColorizeEffect;
	highWarningEffect->setStrength(1.0);
	highWarningEffect->setColor(Qt::darkYellow);
	highWarning->setGraphicsEffect(highWarningEffect);
	highWarning->setWordWrap(true);
}

void Limits::initHighAlarm()
{
	highAlarm = new QLabel(this);
	highAlarm->setFrameStyle(QFrame::VLine || QFrame::Raised);
	
	QGraphicsColorizeEffect * highAlarmEffect = new QGraphicsColorizeEffect;
	highAlarmEffect->setStrength(1.0);
	highAlarmEffect->setColor(Qt::darkRed);
	highAlarm->setGraphicsEffect(highAlarmEffect);
	highAlarm->setWordWrap(true);
}

void Limits::setUnits(const string & _units)
{
	units = " " + _units;	
}

void Limits::setLowAlarm (const double & value)
{
	string str("low alarm: ");
	str += string(doubleToString(value));
	str += units; 
	lowAlarm->setText(QString(str.c_str()));
}

void Limits::setLowWarning (const double & value)
{
	string str("low warning: ");
	str += string(doubleToString(value));
	str += units; 
	lowWarning->setText(QString(str.c_str()));
}

void Limits::setHighWarning (const double & value)
{
	string str("high warning: ");
	str += string(doubleToString(value));
	str += units; 
	highWarning->setText(QString(str.c_str()));
}

void Limits::setHighAlarm (const double & value)
{
	string str("high alarm: ");
	str += string(doubleToString(value));
	str += units; 
	highAlarm->setText(QString(str.c_str()));
}

// SLIDER TAB =====================================================================================

SliderTab::SliderTab(QWidget * parent, Model * _model)
{
	model = _model;

	int rangeLow = (int) model->getRangeMin();
	int rangeHigh = (int) model->getRangeMax();
	int value = model->getValue();

	initProgressBar(rangeLow, rangeHigh, value);
	
	initSlider(rangeLow, rangeHigh, value);
	
	initCheckbox();

	initLimits(rangeLow, rangeHigh);

	connectWidgets();
	
	formatSliderTab();

	setValue(value);
	setUnits();
}

void SliderTab::initProgressBar(
	const int & rangeLow,
	const int & rangeHigh,
	const int & value)
{
	progress_bar = new QProgressBar(this);
	progress_bar->setRange(rangeLow, rangeHigh);
	progress_bar->setValue(value);		

	// Shows the units string next to the current value
	string format = "%p " + model->getUnits(); 
	progress_bar->setFormat(format.c_str());
	
	updateProgressBarColor(value);
}

void SliderTab::initCheckbox()
{
	// The checkbox allows enabling/disabling of the slider's value tracking mode.
	checkBox = new QCheckBox("Slider Value Tracking", this);
	checkBox->setToolTip(tr("Slider value tracking is disabled."));
}

void SliderTab::initSlider(
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

void SliderTab::initLimits(
	const int & rangeLow,
	const int & rangeHigh)
{
	lowLimit = new QLabel(this);
	highLimit = new QLabel(this);

	// Initialize the low limit label
	
	string str = "low limit: ";
	str += string(longToString((long) rangeLow));
	lowLimit->setText(QString(str.c_str()));

	str.clear();

	// Initialize the high limit label
	
	str = "high limit: ";
	str += string(longToString((long) rangeHigh));
	highLimit->setText(QString(str.c_str()));

	// Initialize the limits object that holds other limit data.

	limits = new Limits(this);
	
	limits->setUnits(model->getUnits());
	limits->setLowAlarm(model->getLowAlarm());
	limits->setLowWarning(model->getLowWarning());
	limits->setHighWarning(model->getHighWarning());
	limits->setHighAlarm(model->getHighAlarm());
}

void SliderTab::connectWidgets()
{
	// Connect the slider widget's position to the progress bar widget's value.
	QObject::connect(slider, SIGNAL (sliderMoved(int)), progress_bar, SLOT (setValue(int)));

	// Connect the slider widget's 'value changed' to the view's updateModelValue() function call.
	QObject::connect(slider, SIGNAL (valueChanged(int)), this , SLOT (updateModelValue(int)));

	// Connect the check box's stateChanged signal to the setSliderTracking() function call.
	QObject::connect(checkBox, SIGNAL (stateChanged(int)), this, SLOT(setSliderTracking(int)));
}

void SliderTab::formatSliderTab()
{
	value = new QLabel(this);
	units = new QLabel(this);
	description = new QLabel(this);

	setMinimumSize(450, 200);
	
	QBoxLayout * mainLayout = new QBoxLayout(QBoxLayout::TopToBottom);
	QBoxLayout * subLayout = new QBoxLayout(QBoxLayout::LeftToRight);

	subLayout->addWidget(lowLimit);
	subLayout->setAlignment(lowLimit, Qt::AlignLeft);
	subLayout->addWidget(highLimit);
	subLayout->setAlignment(highLimit, Qt::AlignRight);
	
	mainLayout->addWidget(progress_bar);
	mainLayout->addWidget(checkBox);
	mainLayout->addWidget(slider);
	mainLayout->addLayout(subLayout);
	mainLayout->addWidget(limits);
	mainLayout->addWidget(value);
	mainLayout->addWidget(units);

	setLayout(mainLayout);

	setWindowTitle(tr("qtEpics slider client"));
}

// Calls the model's putValue() method. This writes a new value to the record held
// on the server.
void SliderTab::updateModelValue(const int & value) {
	model->putValue(value);
}

void SliderTab::updateProgressBarColor(const int & value)
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
void SliderTab::setSliderTracking(const int & state)
{
	if (state == Qt::Unchecked) {
		
		slider->setTracking(false);		
		checkBox->setToolTip(tr("Slider value tracking is disabled."));
	
	} else if (state == Qt::Checked) {
	
		slider->setTracking(true);
		checkBox->setToolTip(tr("Slider value tracking is enabled."));
	
	} else {
		// error. checkbox tristate is somehow enabled.	
		slider->setTracking(false);		
	}
}

void SliderTab::setValue(const int & value)
{
	string str = "value: ";
	ostringstream ostr;
	ostr << value;
	str += ostr.str();
	this->value->setText(QString(str.c_str()));
}

void SliderTab::setUnits() {
	string str = "units: ";
	str += model->getUnits();
	units->setText(QString(str.c_str()));
}

void SliderTab::updateData(const int & value)
{
	setValue(value);	
	slider->setValue(value);
	progress_bar->setValue(value);
	updateProgressBarColor(value);
}

// STRUCT TAB =====================================================================================

StructTab::StructTab(QWidget * parent, Model * _model) 
{
	model = _model;

	initTextbox();
	
	QBoxLayout * layout = new QBoxLayout(QBoxLayout::TopToBottom);
	layout->addWidget(textBox);
	setLayout(layout);
}

void StructTab::initTextbox()
{
	textBox = new QTextEdit(this);
	textBox->setReadOnly(true);
	textBox->setMinimumSize(200, 200);
	textBox->setText(getModelText());
}

void StructTab::updateData()
{
	QString text = getModelText();

	textBox->setText(text);	
}

QString StructTab::getModelText()
{
	std::string ret = model->getText();
	return QString(ret.c_str());
}
