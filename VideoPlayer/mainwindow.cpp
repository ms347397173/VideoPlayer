#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    ui->setupUi(this);



    connect(ui->button_openfile,SIGNAL(clicked()),this,SLOT(OnClickedOpenFile()));
    connect(ui->button_play,SIGNAL(clicked()),this,SLOT(OnClickedPlay()));
    connect(ui->button_pause,SIGNAL(clicked()),this,SLOT(OnClickedPause()));
    connect(ui->button_stop,SIGNAL(clicked()),this,SLOT(OnClickedStop()));


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
    if(filePath.isEmpty())
    {
        qDebug()<<"filepath is empty"<<endl;
        return;
    }

    if(vp==NULL)
    {
        vp = new VideoPlay(ui->centralWidget);  //把widget传入VideoPlay对象中
        vp->setGeometry(QRect(0,0,640,480));
        vp->show();
    }

    if(vp->IsInit())
    {
        delete vp;
        vp = new VideoPlay(ui->centralWidget);  //把widget传入VideoPlay对象中
        vp->setGeometry(QRect(0,0,640,480));
        vp->show();
    }

    vp->SetFilePath(filePath);
    qDebug()<<"filepath:"<<vp->GetFilePath()<<endl;

}


void MainWindow::OnClickedPlay()
{
    if(vp==NULL)
    {
        return;
    }
    if(!vp->IsInit())
    {
        if(!vp->Init())
        {
            qDebug()<<"init failde"<<endl;
            return;
        }
        else
        {
            qDebug()<<"init successed"<<endl;
            vp->setGeometry(QRect(0,0,vp->GetWidth(),vp->GetHeight()));
        }
    }

    if(vp->GetPlayState()==STOP)
    {
        qDebug()<<"start play"<<endl;
        vp->start();
        vp->SetPlayState(PLAY);
    }
    else if(vp->GetPlayState()==PAUSE)
    {
        qDebug()<<"continue play"<<endl;
        vp->Play();
    }

}

void MainWindow::OnClickedPause()
{
    if(vp)
    {
        vp->Pause();
    }
}

void MainWindow::OnClickedStop()
{
    if(vp)
    {
        vp->Stop();
    }
    vp->terminate();
    delete vp;
    vp=NULL;
}

