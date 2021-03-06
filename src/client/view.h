
// view.h

// Defines window class for client

#ifndef VIEW_H
#define VIEW_H

#include "model.h"

#include <QWidget>
#include <QTabWidget>
#include <QCheckBox>
#include <QCloseEvent>
#include <QLabel>
#include <QProgressBar>
#include <QSlider>
#include <QTextEdit>

// Widget to display contents of alarm limit structure.
class Limits : public QWidget 
{
	Q_OBJECT

	public: 
		
		explicit Limits(QWidget * Parent = 0);
	
		void setUnits(const std::string & _units);
		void setLowAlarm(const double & value);	
		void setLowWarning(const double & value);	
		void setHighWarning(const double & value);	
		void setHighAlarm(const double & value);	
	
	private:
	
		void initLowAlarm();
		void initLowWarning();
		void initHighWarning();
		void initHighAlarm();
		
		std::string units;

		QLabel * lowAlarm;		// Value at which the low alarm will be triggered.
		QLabel * lowWarning;	// Value at which the low warning will be triggered.
		QLabel * highWarning;	// Value at which the high warning will be triggered.
		QLabel * highAlarm;		// Value at which the high alarm will be triggered.
};

class DataTab : public QWidget
{	
	Q_OBJECT

	public:
		
		explicit DataTab(QWidget * parent = 0, Model * model = 0);
		
		void updateData();

	private:
	
		void initLabels();
		void formatDataTab();

		Model * model; // Object that handles application data

		// Data fields
		QLabel * _long1;
		QLabel * _long2;
		QLabel * _double1;
		QLabel * _double2;
		QLabel * _string1;
		QLabel * _string2;
		QLabel * _boolean;
};

// Tab that displays a single record value along with a slider widget that can control it.
class SliderTab : public QWidget 
{
	Q_OBJECT

	public: 
		
		explicit SliderTab(QWidget * Parent, Model * _model);

		void updateData(const int & value);

	private slots:
	
		// Updates the value on the record through the model.
		void updateModelValue(const int & value);
			
		// Sets the slider's value tracking setting
		void setSliderTracking(const int & state);

	private:
	
		// Private initialization functions called from the constructor
		void initProgressBar(const int & rangeLow,
							const int & rangeHigh,
							const int & value);
		
		void initCheckbox();
		
		void initSlider(const int & rangeLow,
							const int & rangeHigh,
							const int & value);
		
		void initLimits(const int & rangeLow,
							const int & rangeHigh);
		
		// Connects necessary signals and slots of widgets
		void connectWidgets();
		
		// Adds widgets to a layout and sets main window layout
		void formatSliderTab();
		
		// Updates the progress bar's color based on the alarmLimit data in the model. 
		void updateProgressBarColor(const int & value);

		void setValue(const int & value);
		void setUnits();

		QProgressBar * progress_bar;	// Progress bar to show current value in its range
		QLabel * lowLimit;				// Low limit of the value's range
		QLabel * highLimit;				// High limit of the value's range
		QCheckBox * checkBox;			// Check box to enable/disable slider value tracking
		QSlider * slider;				// Slider to control current value in its range
		
		Limits * limits;				// Widget to display data from alarm limit structure
		
		QLabel * value;					// Display record's slider value
		QLabel * units;					// Display record's units string
		QLabel * description;			// Display record's description string
		
		Model * model;					// Object that handles application data	
};

// Tab that displays raw record data.
class StructTab : public QWidget {

	public :
		
		explicit StructTab(QWidget * parent, Model * _model);

		void updateData();

	private:
		
		// Retrieve the textual representation of the record from the model.
		QString getModelText();

		void initTextbox();
		
		QTextEdit * textBox;	// Text box for textual data
		Model * model;			// Object that handles application data	
};

class Window : public QWidget 
{
	Q_OBJECT 

	public:
		
		explicit Window (QWidget * parent = 0);

		// Static wrapper to callback member function
		static void WrapperToCallback(void * ptrToObj, const int & value);
		void closeEvent(QCloseEvent * event);

	signals: 
		
		// Emitted when the value in the model is changed by the model's monitor.
		void valueChanged(const int & value);
	

	private slots:
		
		// Updates the value in the view
		void updateViewValue(const int & value);	

	private:
		
		void callback(const int & value);
		
		Model * model;			// Object that handles application data	

		DataTab   * dataTab;	// Tab contains multiple data fields
		SliderTab * sliderTab;	// Tab contains a slider that manipulates a data field
		StructTab * structTab;	// Tab contains a text box with a record structure dumped into it

		QTabWidget * tabWidget;	

};

#endif // VIEW_H
