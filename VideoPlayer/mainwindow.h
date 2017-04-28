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
    VideoPlay * vp;

private slots:
    void OpenFile();

};

#endif // MAINWINDOW_H
