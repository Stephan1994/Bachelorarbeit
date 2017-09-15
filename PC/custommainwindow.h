#ifndef CUSTOMMAINWINDOW_H
#define CUSTOMMAINWINDOW_H

#include <QWidget>
#include "Robot.h"

class QPushButton;
class QSlider;

class CustomMainWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit CustomMainWindow(QWidget *parent = 0);
	~CustomMainWindow();
	void keyPressEvent(QKeyEvent *key);
	void keyReleaseEvent(QKeyEvent *key);

public slots:
    void slotLeftButtonPressed();
    void slotRightButtonPressed();
    void slotUpButtonPressed();
    void slotDownButtonPressed();
    void slotButtonReleased();
    void slotDisconnect();
	void slotGetDistance();
	
signals:
	void newDistance(int distance);
	
private:
    QPushButton *left;
    QPushButton *right;
    QPushButton *up;
    QPushButton *down;
    Robot rob;
	
	QSlider *distanceSlider;
	QTimer *distanceTimer;
};

#endif // CUSTOMMAINWINDOW_H
