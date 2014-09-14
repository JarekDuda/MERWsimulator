#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MERW.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int w=40, h=40;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:    
    MERW *merw;
    Ui::MainWindow *ui;
    void draw();
    QImage img;
    QRgb val1=qRgb(0,0,0), val2=qRgb(255,255,255), col;

public slots:
    void update();
    void newdef();
    void swMode(bool);
    void stationary(bool);
    void chGradh(int);
    void chGradv(int);
    void chDefp(int);
    void walkers(int);
    void cych(bool);
    void cycv(bool);
    void setwdth(int);
    void setheight(int);
    void pointden();
};

#endif // MAINWINDOW_H
