#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"videoplay.h"
#include <QMainWindow>
#include<QFileDialog>
#include<QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    VideoPlay * vp=NULL;

private slots:
    void OnClickedOpenFile();
    void OnClickedPlay();
    void OnClickedPause();
    void OnClickedStop();

};

#endif // MAINWINDOW_H
