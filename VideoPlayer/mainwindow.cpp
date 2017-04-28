#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //创建对象
    vp = new VideoPlay(ui->VideoWindow);
    connect(ui->button_openfile,SIGNAL(clicked()),this,SLOT(OpenFile()));

}

MainWindow::~MainWindow()
{
    delete ui;
    //删除、析构对象
    delete vp;
}


void MainWindow::OpenFile()
{
    QString filePath=QFileDialog::getOpenFileName(NULL,QString(),QString());
    vp->SetFilePath(filePath);
    qDebug()<<"filepath:"<<vp->GetFilePath()<<endl;
}
