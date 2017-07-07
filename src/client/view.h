
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

// Tab that displays formatted record data. 
class DataTab : public QWidget 
{
	Q_OBJECT

	public: 
		
		explicit DataTab(QWidget * Parent, Model * _model);

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
		void initLimits();
		
		// Connects necessary signals and slots of widgets
		void connectWidgets();
		
		// Adds widgets to a layout and sets main window layout
		void formatDataTab();
		
		// Updates the progress bar's color based on the alarmLimit data in the model. 
		void updateProgressBarColor(const int & value);

		void setValue(const int & value);
		void setUnits();

		QProgressBar * progress_bar;	// Progress bar to show current value in its range
		QCheckBox * checkBox;			// Check box to enable/disable slider value tracking
		QSlider * slider;				// Slider to control current value in its range
		
		Limits * limits;				// Widget to display data from alarm limit structure
		
		QLabel * value;					// Display record's scalar value
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
		
		Model * model;

		DataTab * dataTab;
		StructTab * structTab;

		QTabWidget * tabWidget;	

};

#endif // VIEW_H
