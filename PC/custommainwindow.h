#ifndef CUSTOMMAINWINDOW_H
#define CUSTOMMAINWINDOW_H

#include <QWidget>
#include "Robot.h"

class QPushButton;
class CustomMainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CustomMainWindow(QWidget *parent = 0);

signals:

public slots:
    void slotLeftButtonPressed();
    void slotRightButtonPressed();
    void slotUpButtonPressed();
    void slotDownButtonPressed();
    void slotButtonReleased();
    void slotDisconnect();
private:
    QPushButton *left;
    QPushButton *right;
    QPushButton *up;
    QPushButton *down;
    Robot rob;
};

#endif // CUSTOMMAINWINDOW_H
