
// view.h

// Defines window class for client

#ifndef VIEW_H
#define VIEW_H

#include "model.h"

#include <QWidget>
#include <QProgressBar>
#include <QSlider>
#include <QTextEdit>
#include <QCloseEvent>
#include <QCheckBox>

class Window : public QWidget 
{
	Q_OBJECT

	public: 
		explicit Window(QWidget * Parent = 0);
	
		// Static wrapper to callback member function
		static void WrapperToCallback(void * ptrToObj, const int & value);
		void callback(const int & value);
		void closeEvent(QCloseEvent * event);

	private slots:
	
		// Updates the value on the record through the model.
		void updateModelValue(const int & value);
		
		// Updates the value in the view
		void updateViewValue(const int & value);	

		// Sets the slider's value tracking setting
		void setSliderTracking(const int & state);

	signals: 
		// Emitted when the value in the model is changed by the model's monitor.
		void valueChanged(const int & value);
	
	private:
		QString getModelText();

		QTextEdit * textBox;			// Text box for textual data
		QProgressBar * progress_bar;	// Progress bar to show current value in its range
		QSlider * slider;				// Slider to control current value in its range
		QCheckBox * checkBox;			// Check box to enable/disable slider value tracking
		Model * model;					// Object that handles application data	
};

#endif // VIEW_H
