#ifndef VIEW_H
#define VIEW_H

#include "model.h"

#include <QWidget>
#include <QProgressBar>
#include <QSlider>

class Window : public QWidget 
{
	Q_OBJECT

	public: 
		explicit Window(QWidget * Parent = 0);
	
		static void WrapperToCallback(void * ptrToObj, const int & value);
		void callback(const int & value);

	private slots:
	/* Updates the value on the record through the model. */
		void updateModelValue(const int & value);
	/* Updates the value in the view */
		void updateViewValue(const int & value);	
	
	signals: 
	/* Emitted when the value in the model is changed by the model's monitor. */
		void valueChanged(const int & value);
	
	private:
	
		QProgressBar * progress_bar;
		QSlider * slider;

		Model * model;		
};

#endif /* VIEW_H */
