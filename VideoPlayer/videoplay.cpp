#include"videoplay.h"

void VideoPlay::Paly()
{}
void VideoPlay::Pause()
{}
void VideoPlay::Stop()
{}


VideoPlay::VideoPlay(QWidget * widget)
    :QObject(NULL)
    ,_widget(widget)
{}

VideoPlay::VideoPlay(QWidget * widget,QString filePath)
    :QObject(NULL)
    ,_widget(widget)
    ,_filePath(filePath)
{}

VideoPlay::~VideoPlay()
{
    UnInit();
}

bool VideoPlay::Init()
{
    //QByteArray ba=_filePath.toLatin1();
    QByteArray ba=_filePath.toLocal8Bit();
    const char * filePath=ba.data();

    _decoder=new VideoDecoder;
    if(!_decoder->Init(filePath))
    {
        return false;
    }
    _sdlHandle=new SDLDispaly;
    if(!_sdlHandle->Init())
    {
        return false;
    }
    return true;
}

bool VideoPlay::UnInit()
{
    bool ret=true;
    if(!_decoder->UnInit())
    {
        ret=false;
    }
    delete _decoder;
    _decoder=NULL;

    if(!_sdlHandle->UnInit())
    {
         ret=false;
    }
    delete _sdlHandle;
    _sdlHandle=NULL;

    return ret;
}

QString VideoPlay::SetFilePath(QString newFilePath)
{
    swap(this->_filePath,newFilePath);
    return newFilePath;
}

QString VideoPlay::GetFilePath()
{
    return this->_filePath;
}

enum play_state_type VideoPlay::SetPlayState(enum play_state_type newPlayState)
{
    enum play_state_type oldState;
    oldState=this->_playState;
    this->_playState=newPlayState;

    return oldState;
}
enum play_state_type VideoPlay::GetPlayState()
{
    return this->_playState;
}
