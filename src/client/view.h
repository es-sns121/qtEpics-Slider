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
	
	private slots:
	/* Updates the value on the record. */
		void updateValue(const int & value);

	private:
	
		QProgressBar * progress_bar;
		QSlider * slider;

		Model * model;		
};

#endif /* VIEW_H */
