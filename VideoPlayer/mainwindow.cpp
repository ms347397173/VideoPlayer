#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //创建对象
    vp = new VideoPlay(NULL);  //把widget传入VideoPlay对象中
    connect(ui->button_openfile,SIGNAL(clicked()),this,SLOT(OnClickedOpenFile()));
    connect(ui->button_play,SIGNAL(clicked()),this,SLOT(OnClickedPlay()));


}

MainWindow::~MainWindow()
{
    delete ui;
    //删除、析构对象
    delete vp;
    vp=NULL;
}


void MainWindow::OnClickedOpenFile()
{
    QString filePath=QFileDialog::getOpenFileName(NULL,QString(),QString());
    vp->SetFilePath(filePath);
    qDebug()<<"filepath:"<<vp->GetFilePath()<<endl;
}


void MainWindow::OnClickedPlay()
{

    if(vp->GetFilePath().isEmpty())
    {
        qDebug()<<"no file"<<endl;
        return;
    }

    vp->start();

}
